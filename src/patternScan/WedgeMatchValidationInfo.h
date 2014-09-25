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

    // Return an end to the potential pattern match. This is an ending iterator
    // in the same sense as STL's standard end() for collections. In other words,
    // don't reference the underlying PeriodValCltn using what is returned from
    // this function.
    const PeriodValCltn::iterator patternEndIter() const;

    ChartSegmentPtr createWedgeSegment() const;

    // Returns a non-null pointer to the breakout info if a breakout/breakdown exists,
    // otherwise returns the null smart pointer.
    PatternMatchBreakoutInfoPtr upperTrendLineBreakout() const;
    PatternMatchBreakoutInfoPtr lowerTrendLineBreakdown() const;

    bool incompletePatternMatch() const;
};

#endif // WEDGEMATCHVALIDATIONINFO_H
