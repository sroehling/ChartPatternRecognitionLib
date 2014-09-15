#include "DescendingTrianglePatternMatch.h"
#include "PatternMatchVisitor.h"

DescendingTrianglePatternMatch::DescendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : WedgePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void DescendingTrianglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    WedgePatternMatch::acceptVisitor(visitor);
    visitor.visitFallingWedgePatternMatch(*this);
}

