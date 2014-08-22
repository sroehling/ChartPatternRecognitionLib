#include "SecondPeriodValuePivotsLower.h"

SecondPeriodValuePivotsLower::SecondPeriodValuePivotsLower()
{
}


bool SecondPeriodValuePivotsLower::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);
    PeriodValCltn::iterator matchIter = candidateMatch.matchSegment()->segBegin();

    double firstVal = (*matchIter).typicalPrice();

    matchIter++;
    double secondVal = (*matchIter).typicalPrice();

    return (firstVal < secondVal)? true : false;
}
