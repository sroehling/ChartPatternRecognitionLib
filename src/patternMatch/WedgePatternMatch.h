#ifndef WEDGEPATTERNMATCH_H
#define WEDGEPATTERNMATCH_H

#include "PatternMatch.h"
#include "ChartSegment.h"

class WedgePatternMatch: public PatternMatch {
private:
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;
public:
    WedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    ChartSegmentPtr upperTrendLine() const { return upperTrendLine_; }
    ChartSegmentPtr lowerTrendLine() const { return lowerTrendLine_; }

    // Iterators defining the beginning and end of the trend lines
    // comprising the pattern. These methods are useful for iterating
    // through the period values and drawing the lower and upper
    // trend lines.
    PeriodValCltn::iterator patternBeginIter() const;
    PeriodValCltn::iterator interceptEndIter() const;

    virtual ~WedgePatternMatch() {}
};


#endif // WEDGEPATTERNMATCH_H
