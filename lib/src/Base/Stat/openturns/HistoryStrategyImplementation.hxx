//                                               -*- C++ -*-
/**
 *  @brief This class is a top-level class for the history mechanism of
 *  simulation algorithms. It delegates to its children the effective
 *  history strategy.
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
#ifndef OPENTURNS_HISTORYSTRATEGYIMPLEMENTATION_HXX
#define OPENTURNS_HISTORYSTRATEGYIMPLEMENTATION_HXX

#include "openturns/PersistentObject.hxx"
#include "openturns/Point.hxx"
#include "openturns/Sample.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class HistoryStrategyImplementation
 */

class OT_API HistoryStrategyImplementation
  : public PersistentObject
{

  CLASSNAME;

public:


  /** Constructor with parameters */
  HistoryStrategyImplementation();

  /** Virtual constructor */
  virtual HistoryStrategyImplementation * clone() const;

  /** Store the point according to the strategy */
  virtual void store(const Point & point);
  virtual void store(const Sample & sample);

  /** Clear the history storage */
  void clear();

  /** History sample accessor */
  virtual Sample getSample() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  /** Data container */
  Sample sample_;

  /** Initialization flag */
  Bool isInitialized_;
} ; /* class HistoryStrategyImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_HISTORYSTRATEGYIMPLEMENTATION_HXX */
