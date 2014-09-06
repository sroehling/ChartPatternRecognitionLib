#include "HighestHighLessThanFirstHigh.h"
#include <boost/log/trivial.hpp>

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

    BOOST_LOG_TRIVIAL(debug) << "HighestHighLessThanFirstHigh: first high: " << firstHigh
                                << " highest high: " << highestHigh;


    return (highestHigh <= firstHighWithTolerance)? true : false;
}
