//                                               -*- C++ -*-
/**
 *  @brief Factory for Arcsine distribution
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
#include "openturns/ArcsineFactory.hxx"
#include "openturns/ArcsineMuSigma.hxx"
#include "openturns/PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ArcsineFactory);

static const Factory<ArcsineFactory> Factory_ArcsineFactory;

/* Default constructor */
ArcsineFactory::ArcsineFactory()
  : DistributionFactoryImplementation()
{
  // Initialize any other class members here
  // At last, allocate memory space if needed, but go to destructor to free it
}

/* Virtual constructor */
ArcsineFactory * ArcsineFactory::clone() const
{
  return new ArcsineFactory(*this);
}


/* Here is the interface that all derived class must implement */

ArcsineFactory::Implementation ArcsineFactory::build(const Sample & sample) const
{
  return buildAsArcsine(sample).clone();
}

ArcsineFactory::Implementation ArcsineFactory::build(const Point & parameters) const
{
  return buildAsArcsine(parameters).clone();
}

ArcsineFactory::Implementation ArcsineFactory::build() const
{
  return buildAsArcsine().clone();
}

DistributionFactoryResult ArcsineFactory::buildEstimator(const Sample & sample) const
{
  return buildBootStrapEstimator(sample, true);
}

Arcsine ArcsineFactory::buildAsArcsine(const Sample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Arcsine distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Arcsine distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  Scalar mean = sample.computeMean()[0];
  Scalar standardDeviation = sample.computeStandardDeviationPerComponent()[0];
  Point parameters(2);
  parameters[0] = mean;
  parameters[1] = standardDeviation;
  Arcsine result(buildAsArcsine(ArcsineMuSigma()(parameters)));
  result.setDescription(sample.getDescription());
  return result;
}

Arcsine ArcsineFactory::buildAsArcsine(const Point & parameters) const
{
  try
  {
    Arcsine distribution;
    distribution.setParameter(parameters);
    return distribution;
  }
  catch (InvalidArgumentException)
  {
    throw InvalidArgumentException(HERE) << "Error: cannot build a Arcsine distribution from the given parameters";
  }
}

Arcsine ArcsineFactory::buildAsArcsine() const
{
  return Arcsine();
}


END_NAMESPACE_OPENTURNS
