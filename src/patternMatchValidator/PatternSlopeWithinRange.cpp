#include "PatternSlopeWithinRange.h"
#include "SlopeWithinRangeConstraint.h"

PatternSlopeWithinRange::PatternSlopeWithinRange(const DoubleRange &slopeRange)
    : slopeRange_(slopeRange)
{
}


bool PatternSlopeWithinRange::validPattern(const PatternMatch &candidateMatch)
{
    return SlopeWithinRangeConstraint(slopeRange_).validSegment(candidateMatch.matchSegment());
}
