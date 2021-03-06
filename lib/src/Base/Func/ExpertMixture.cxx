//                                               -*- C++ -*-
/**
 *  @brief The class building chaos expansions
 *
 *  Copyright 2005-2017 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "openturns/ExpertMixture.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ExpertMixture);

static const Factory<ExpertMixture> Factory_ExpertMixture;


/* Default constructor */
ExpertMixture::ExpertMixture()
  : EvaluationImplementation()
  , experts_()
  , classifier_()
{
  // Nothing to do
}


/* Constructor */
ExpertMixture::ExpertMixture(const Basis & experts,
                             const Classifier & classifier)
  : EvaluationImplementation()
  , experts_(experts)
  , classifier_(classifier)
{
  // Check if there is at least one expert
  if (experts.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build an ExpertMixture with no expert!";
  // Check that the experts and the classifier are compatible
  if (classifier.getDimension() != (experts[0].getInputDimension() + experts[0].getOutputDimension())) throw InvalidArgumentException(HERE) << "Error: the classifier dimension must be equal to the sum of the experts input and output dimensions, here the classifier dimension=" << classifier.getDimension() << " and the experts input dimension=" << experts[0].getInputDimension() << " and output dimension=" << experts[0].getOutputDimension();
  setDescription(experts[0].getDescription());
}

/* Virtual constructor */
ExpertMixture * ExpertMixture::clone() const
{
  return new ExpertMixture(*this);
}

/* Comparison operator */
Bool ExpertMixture::operator ==(const ExpertMixture & other) const
{
  if (this == &other) return true;
  return (experts_ == other.experts_) && (classifier_ == other.classifier_);
}

/* String converter */
String ExpertMixture::__repr__() const
{
  return OSS(true) << "class=" << getClassName()
         << ", experts=" << experts_
         << ", classifier=" << classifier_;
}

String ExpertMixture::__str__(const String & offset) const
{
  return OSS(false) << "experts=" << experts_.__str__(offset)
         << "\nclassifier=" << classifier_.__str__(offset);
}


/* Local experts accessor */
void ExpertMixture::setExperts(const Basis & experts)
{
  if (experts.getDimension() != classifier_.getDimension()) throw InvalidArgumentException(HERE) << "Error: the local experts must have the same input dimension as the classifier dimension. Here, experts input dimension=" << experts.getDimension() << " and classifier dimension=" << classifier_.getDimension();
  experts_ = experts;
}

Basis ExpertMixture::getExperts() const
{
  return experts_;
}

/* Classifier accessor */
void ExpertMixture::setClassifier(const Classifier & classifier)
{
  if (experts_.getDimension() != classifier.getDimension()) throw InvalidArgumentException(HERE) << "Error: the local experts must have the same input dimension as the classifier dimension. Here, experts input dimension=" << experts_.getDimension() << " and classifier dimension=" << classifier.getDimension();
  classifier_ = classifier;
}

Classifier ExpertMixture::getClassifier() const
{
  return classifier_;
}


/* Operator () */
Point ExpertMixture::operator() (const Point & inP) const
{
  const UnsignedInteger inputDimension = getInputDimension();
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension << " and got a point of dimension=" << inP.getDimension();
  const UnsignedInteger outputDimension = getOutputDimension();
  const UnsignedInteger size = experts_.getSize();
  UnsignedInteger bestClass = 0;
  // Build the point (x, f(x)) for the first class and grade it according to the classifier
  Point mixedPoint(inP);
  Point bestValue(experts_[0](inP));
  mixedPoint.add(bestValue);
  Scalar bestGrade = classifier_.grade(mixedPoint, bestClass);
  LOGDEBUG(OSS() << "Class index=" << 0 << ", grade=" << bestGrade << ", value=" << bestValue);
  for (UnsignedInteger classIndex = 1; classIndex < size; ++classIndex)
  {
    // Build the point (x, f(x)) for each other class and grade it according to the classifier
    const Point localValue(experts_[classIndex](inP));
    for (UnsignedInteger i = 0; i < outputDimension; ++i) mixedPoint[inputDimension + i] = localValue[i];
    const Scalar grade = classifier_.grade(mixedPoint, classIndex);
    LOGDEBUG(OSS() << "Class index=" << classIndex << ", grade=" << grade << ", value=" << localValue);
    // The best class will give the output value
    if (grade > bestGrade)
    {
      bestGrade = grade;
      bestClass = classIndex;
      bestValue = localValue;
    }
  }
  LOGDEBUG(OSS() << "Best class index=" << bestClass << ", best grade=" << bestGrade << ", best value=" << bestValue);
  return bestValue;
}

Sample ExpertMixture::operator() (const Sample & inS) const
{
  const UnsignedInteger inputDimension = getInputDimension();
  if (inS.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: expected a point of dimension=" << inputDimension << " and got a sample of dimension=" << inS.getDimension();
  const UnsignedInteger size = inS.getSize();
  const UnsignedInteger outputDimension = getOutputDimension();
  Sample bestValues(size, outputDimension);
  const UnsignedInteger expertSize = experts_.getSize();
  Point bestGrades(size, -SpecFunc::MaxScalar);
  for (UnsignedInteger classIndex = 0; classIndex < expertSize; ++classIndex)
  {
    // Build the point (x, f(x)) for each other class and grade it according to the classifier
    Sample mixedSample(inS);
    // Here is the evaluation of the expert over a sample, benefiting from possible
    // parallelism/vectorization
    const Sample localValues(experts_[classIndex](inS));
    mixedSample.stack(localValues);
    const Point grades = classifier_.grade(mixedSample, Indices(size, classIndex));
    for (UnsignedInteger i = 0; i < size; ++i)
      if (grades[i] > bestGrades[i])
      {
        bestGrades[i] = grades[i];
        bestValues[i] = localValues[i];
      }
  } // classIndex
  return bestValues;
}

/* Accessor for input point dimension */
UnsignedInteger ExpertMixture::getInputDimension() const
{
  return experts_[0].getInputDimension();
}

/* Accessor for output point dimension */
UnsignedInteger ExpertMixture::getOutputDimension() const
{
  return experts_[0].getOutputDimension();
}


/* Method save() stores the object through the StorageManager */
void ExpertMixture::save(Advocate & adv) const
{
  EvaluationImplementation::save(adv);
  adv.saveAttribute( "experts_", experts_ );
  adv.saveAttribute( "classifier_", classifier_ );
}


/* Method load() reloads the object from the StorageManager */
void ExpertMixture::load(Advocate & adv)
{
  EvaluationImplementation::load(adv);
  adv.loadAttribute( "experts_", experts_ );
  adv.loadAttribute( "classifier_", classifier_ );
}

END_NAMESPACE_OPENTURNS
