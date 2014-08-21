#include "LowestLowGreaterThanLastLow.h"

LowestLowGreaterThanLastLow::LowestLowGreaterThanLastLow()
{
}

bool LowestLowGreaterThanLastLow::validPattern(const PatternMatch &candidateMatch)
{
    double lowestLow = candidateMatch.lowestLow();
    double lastLow = candidateMatch.lastValue().low();
    return (lastLow <= lowestLow)? true : false;
}
