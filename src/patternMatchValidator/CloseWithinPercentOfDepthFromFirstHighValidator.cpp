#include "CloseWithinPercentOfDepthFromFirstHighValidator.h"
#include "PatternMatch.h"

CloseWithinPercentOfDepthFromFirstHighValidator::CloseWithinPercentOfDepthFromFirstHighValidator(double percDepthThreshold)
    : percDepthThreshold_(percDepthThreshold)
{
    assert(percDepthThreshold >= -2.0); // -300%
    assert(percDepthThreshold <= 2.0); // +300%
}

bool CloseWithinPercentOfDepthFromFirstHighValidator::validPattern(const PatternMatch &candidateMatch)
{
    double firstHigh = candidateMatch.firstValue().high();
    double lowestLow = candidateMatch.lowestLow();
    assert(firstHigh >= lowestLow);
    double depth = firstHigh-lowestLow;

    double thresholdHigh = firstHigh + (depth * percDepthThreshold_);

    double lastClose = candidateMatch.lastValue().close();

    return (lastClose <= thresholdHigh) ? true: false;
}
