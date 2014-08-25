#include "FallingWedgePatternMatch.h"
#include "PatternMatchVisitor.h"

FallingWedgePatternMatch::FallingWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : WedgePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void FallingWedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    WedgePatternMatch::acceptVisitor(visitor);
    visitor.visitFallingWedgePatternMatch(*this);
}

