#include "PatternSegmentValsCloseToLinearEq.h"
#include "SegmentValsCloseToLinearEq.h"

PatternSegmentValsCloseToLinearEq::PatternSegmentValsCloseToLinearEq(double maxDeltaPercVsEquation)
    : maxDeltaPercVsEquation_(maxDeltaPercVsEquation)
{
}


bool PatternSegmentValsCloseToLinearEq::validPattern(const PatternMatch &candidateMatch)
{
    return SegmentValsCloseToLinearEq(maxDeltaPercVsEquation_).validSegment(candidateMatch.matchSegment());
}
