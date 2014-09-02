#include "PercentIntersectingPatternLineValidator.h"
#include "LinearEquation.h"
#include "ChartSegment.h"


#define PERC_INTERSECTING_LINE_DEFAULT_PERC 0.6

PercentIntersectingPatternLineValidator::PercentIntersectingPatternLineValidator()
{
}


bool PercentIntersectingPatternLineValidator::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);
    assert(candidateMatch.segments().size() ==1); // Only works with single-segment pattern matches

    PeriodValCltn::iterator segBegin = candidateMatch.matchSegment()->segBegin();
    PeriodValCltn::iterator segEnd = candidateMatch.matchSegment()->segEnd();

    LinearEquationPtr patternEq = candidateMatch.segments().front()->segmentEq();

    double totalIntersecting = 0.0;
    double totalValues = 0.0;

    for(PeriodValCltn::iterator segIter = segBegin; segIter != segEnd; segIter++)
    {
        if(patternEq->aboveLine((*segIter).highCoord()) &&
                patternEq->belowLine((*segIter).lowCoord()))
        {
            totalIntersecting += 1.0;
        }
        totalValues += 1.0;
    }

    assert(totalValues > 0.0);


    double percIntersect = totalIntersecting/totalValues;

    if(percIntersect > PERC_INTERSECTING_LINE_DEFAULT_PERC)
    {
        return true;
    }
    else
    {
        return false;
    }

}
