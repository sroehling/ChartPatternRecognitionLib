#include "HighestHighLessThanFirstHigh.h"
#include "DebugLog.h"

HighestHighLessThanFirstHigh::HighestHighLessThanFirstHigh()
{
}


bool HighestHighLessThanFirstHigh::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);
    PeriodValCltn::iterator matchIter = candidateMatch.matchSegment()->segBegin();

    double firstHigh = (*matchIter).high();
    double firstHighWithTolerance = firstHigh + 0.001;

    double highestHigh = candidateMatch.highestHigh();

    DEBUG_MSG("HighestHighLessThanFirstHigh: first high: " << firstHigh
                                << " highest high: " << highestHigh);


    return (highestHigh <= firstHighWithTolerance)? true : false;
}
