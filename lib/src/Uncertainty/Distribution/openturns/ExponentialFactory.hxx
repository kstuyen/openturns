//                                               -*- C++ -*-
/**
 *  @brief Factory for Exponential distribution
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
#ifndef OPENTURNS_EXPONENTIALFACTORY_HXX
#define OPENTURNS_EXPONENTIALFACTORY_HXX

#include "openturns/OTprivate.hxx"
#include "openturns/DistributionFactoryImplementation.hxx"
#include "openturns/Exponential.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ExponentialFactory
 */
class OT_API ExponentialFactory
  : public DistributionFactoryImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  ExponentialFactory();

  /** Virtual constructor */
  virtual ExponentialFactory * clone() const;

  /* Here is the interface that all derived class must implement */
  using DistributionFactoryImplementation::build;

  Implementation build(const Sample & sample) const;
  Implementation build(const Point & parameters) const;
  Implementation build() const;
  Exponential buildAsExponential(const Sample & sample) const;
  Exponential buildAsExponential(const Point & parameters) const;
  Exponential buildAsExponential() const;

}; /* class ExponentialFactory */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_EXPONENTIALFACTORY_HXX */
