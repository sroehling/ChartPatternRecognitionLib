#include "PatternMatchBreakoutInfo.h"
#include <assert.h>

PatternMatchBreakoutInfo::PatternMatchBreakoutInfo(double pseudoXVal, double breakoutPrice)
    : periodValPseudoXVal_(pseudoXVal), breakoutPrice_(breakoutPrice)
{
    assert(pseudoXVal >= 0.0);
    assert(breakoutPrice >= 0.0);
}
