#include "RisingWedgePatternMatch.h"
#include "PatternMatchVisitor.h"

RisingWedgePatternMatch::RisingWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : WedgePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void RisingWedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    WedgePatternMatch::acceptVisitor(visitor);
    visitor.visitRisingWedgePatternMatch(*this);
}
