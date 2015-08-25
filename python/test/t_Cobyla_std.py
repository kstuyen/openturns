#! /usr/bin/env python

from __future__ import print_function
from openturns import *

TESTPREAMBLE()

try:

    try:
        # Test function operator ()
        levelFunction = NumericalMathFunction(
            ["x1", "x2", "x3", "x4"], ["y1"], ["x1+2*x2-3*x3+4*x4"])
        specific = CobylaSpecificParameters()
        startingPoint = NumericalPoint(4, 1.0)
        myAlgorithm = Cobyla(specific, levelFunction)
        myAlgorithm.setStartingPoint(startingPoint)
        myAlgorithm.setLevelValue(3.0)
        myAlgorithm.setMaximumIterationsNumber(100)
        myAlgorithm.setMaximumAbsoluteError(1.0e-10)
        myAlgorithm.setMaximumRelativeError(1.0e-10)
        myAlgorithm.setMaximumResidualError(1.0e-10)
        myAlgorithm.setMaximumConstraintError(1.0e-10)
        print("myAlgorithm = ", repr(myAlgorithm))
    except:
        raise

except:
    import sys
    print("t_Cobyla_std.py", sys.exc_info()[0], sys.exc_info()[1])
