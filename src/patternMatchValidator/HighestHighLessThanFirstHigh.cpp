#include "HighestHighLessThanFirstHigh.h"

HighestHighLessThanFirstHigh::HighestHighLessThanFirstHigh()
{
}


bool HighestHighLessThanFirstHigh::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);
    PeriodValCltn::iterator matchIter = candidateMatch.matchSegment()->segBegin();

    double firstHigh = (*matchIter).high();
    double firstHighWithTolerance = firstHigh + 0.001;

    matchIter++;
    double highestHigh = candidateMatch.highestHigh();

    return (highestHigh <= firstHighWithTolerance)? true : false;
}
