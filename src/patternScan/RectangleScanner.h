#ifndef RECTANGLESCANNER_H
#define RECTANGLESCANNER_H

#include "UnsignedIntRange.h"
#include "WedgeScannerEngine.h"

class RectangleScanner : public WedgeScannerEngine
{
private:
    UnsignedIntRange validPeriodRange_;
public:
    RectangleScanner(const UnsignedIntRange &validPeriodRange);
    RectangleScanner();

    virtual bool validTrendLines(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;

    // Calculate the minimum and maximum number of periods for a valid
    // pattern match.
    virtual unsigned int maxPatternPeriods(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;
    virtual unsigned int minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;

    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;

};

#endif // RECTANGLESCANNER_H
