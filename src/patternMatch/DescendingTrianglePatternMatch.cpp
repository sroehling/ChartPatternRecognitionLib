#include "DescendingTrianglePatternMatch.h"
#include "PatternMatchVisitor.h"

DescendingTrianglePatternMatch::DescendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : TrianglePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void DescendingTrianglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    TrianglePatternMatch::acceptVisitor(visitor);
    visitor.visitDescendingTrianglePatternMatch(*this);
}

