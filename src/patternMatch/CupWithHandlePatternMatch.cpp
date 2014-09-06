#include "CupWithHandlePatternMatch.h"
#include "PatternMatchVisitor.h"


CupWithHandlePatternMatch::CupWithHandlePatternMatch(const PatternMatchPtr &cup, const PatternMatchPtr &handle)
    : PatternMatch(cup->appendMatch(*handle)->segments()),
      cup_(cup),
      handle_(handle)
{
    PatternMatch::initBreakoutAboveFirstHighBreakoutInfo(handle);
}


void CupWithHandlePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    cup_->acceptVisitor(visitor);

    visitor.visitCupWithHandleHandleStart(*this);

    handle_->acceptVisitor(visitor);

    visitor.visitCupWithHandlePatternMatch(*this);
}
