#include "CupPatternMatch.h"
#include "PatternMatchVisitor.h"

CupPatternMatch::CupPatternMatch(const PatternMatchPtr &downTrend, const PatternMatchPtr &cupBottom,
                                 const PatternMatchPtr &upTrend)
    : PatternMatch(downTrend->appendMatch(*cupBottom)->appendMatch(*upTrend)->segments()),
      downTrend_(downTrend),
      cupBottom_(cupBottom),
      upTrend_(upTrend)
{
}


void CupPatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    downTrend_->acceptVisitor(visitor);
    cupBottom_->acceptVisitor(visitor);
    upTrend_->acceptVisitor(visitor);
    visitor.visitCupPatternMatch(*this);
}
