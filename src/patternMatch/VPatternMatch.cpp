#include "VPatternMatch.h"
#include "PatternMatchVisitor.h"

VPatternMatch::VPatternMatch(const PatternMatchPtr &downTrend, const PatternMatchPtr upTrend)
    : PatternMatch(downTrend->appendMatch(*upTrend)->segments()),
      downTrend_(downTrend),
    upTrend_(upTrend)
{
}


void VPatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    downTrend_->acceptVisitor(visitor);
    upTrend_->acceptVisitor(visitor);
    visitor.visitVPatternMatch(*this);
}
