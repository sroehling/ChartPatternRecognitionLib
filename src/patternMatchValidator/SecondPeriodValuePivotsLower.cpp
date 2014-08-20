#include "SecondPeriodValuePivotsLower.h"

SecondPeriodValuePivotsLower::SecondPeriodValuePivotsLower()
{
}


bool SecondPeriodValuePivotsLower::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);
    PeriodValCltn::iterator matchIter = candidateMatch.matchSegment()->segBegin();

    double firstHigh = (*matchIter).high();

    matchIter++;
    double secondHigh = (*matchIter).high();

    return (secondHigh < firstHigh)? true : false;
}
