#include "AscendingTrianglePatternMatch.h"
#include "PatternMatchVisitor.h"

AscendingTrianglePatternMatch::AscendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : WedgePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void AscendingTrianglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    WedgePatternMatch::acceptVisitor(visitor);
    visitor.visitRisingWedgePatternMatch(*this);
}
