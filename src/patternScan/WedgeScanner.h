#ifndef WEDGESCANNER_H
#define WEDGESCANNER_H

#include "DoubleRange.h"
#include "UnsignedIntRange.h"
#include "WedgeScannerEngine.h"

class WedgeScanner : public WedgeScannerEngine
{
private:
    DoubleRange upperTrendLineSlopeRange_;
    DoubleRange lowerTrendLineSlopeRange_;
    UnsignedIntRange validPeriodRange_;
public:
    WedgeScanner(const DoubleRange &upperTrendLineSlopeRange,
                 const DoubleRange &lowerTrendLineSlopeRange);

    virtual bool validTrendLines(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;

    // Calculate the minimum and maximum number of periods for a valid
    // pattern match.
    virtual unsigned int maxPatternPeriods(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;
    virtual unsigned int minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;

    // The findPatternMatch method is still virtual

};

#endif // WEDGESCANNER_H
