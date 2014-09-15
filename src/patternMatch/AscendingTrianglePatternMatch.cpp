#include "AscendingTrianglePatternMatch.h"
#include "PatternMatchVisitor.h"

AscendingTrianglePatternMatch::AscendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : TrianglePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void AscendingTrianglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    TrianglePatternMatch::acceptVisitor(visitor);
    visitor.visitAscendingTrianglePatternMatch(*this);
}
