#ifndef TRIANGLEPATTERNMATCH_H
#define TRIANGLEPATTERNMATCH_H

#include "PatternMatch.h"
#include "ChartSegment.h"

class TrianglePatternMatch: public PatternMatch {
private:
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;

public:
    TrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    ChartSegmentPtr upperTrendLine() const { return upperTrendLine_; }
    ChartSegmentPtr lowerTrendLine() const { return lowerTrendLine_; }

    XYCoord trendLineIntercept() const;


    // Iterators defining the beginning and end of the trend lines
    // comprising the pattern. These methods are useful for iterating
    // through the period values and drawing the lower and upper
    // trend lines.
    PeriodValCltn::iterator patternBeginIter() const;
    PeriodValCltn::iterator interceptEndIter() const;

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual ~TrianglePatternMatch() {}
};


#endif // WEDGEPATTERNMATCH_H
