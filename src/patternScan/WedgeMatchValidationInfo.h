#ifndef WEDGEMATCHVALIDATIONINFO_H
#define WEDGEMATCHVALIDATIONINFO_H

#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "PatternMatchBreakoutInfo.h"


class WedgeMatchValidationInfo
{
private:
    PeriodValSegmentPtr chartVals_;
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;
    PeriodValCltn::iterator currPerValIter_;
public:
    WedgeMatchValidationInfo(const PeriodValSegmentPtr &chartVals,
                             const ChartSegmentPtr &upperTrendLine,
                             const ChartSegmentPtr &lowerTrendLine,
                             const PeriodValCltn::iterator &currPerValIter);

    const PeriodValSegmentPtr &chartVals() const { return chartVals_; }
    const ChartSegmentPtr &upperTrendLine() const { return upperTrendLine_; }
    const ChartSegmentPtr &lowerTrendLine() const { return lowerTrendLine_; }
    const PeriodValCltn::iterator &currPerValIter() const { return currPerValIter_; }
    const PeriodValCltn::iterator patternBeginIter() const;
    const PeriodValCltn::iterator patternEndIter() const;
    ChartSegmentPtr createWedgeSegment() const;

    // Returns a non-null pointer to the breakout info if a breakout exists, otherwise
    // returns the null smart pointer.
    PatternMatchBreakoutInfoPtr upperTrendLineBreakout() const;
};

#endif // WEDGEMATCHVALIDATIONINFO_H
