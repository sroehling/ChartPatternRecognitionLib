#include "CupWithoutHandlePatternMatch.h"
#include "PatternMatchVisitor.h"

CupWithoutHandlePatternMatch::CupWithoutHandlePatternMatch(const PatternMatchPtr &cup)
    : PatternMatch(cup->segments()),
      cup_(cup)
{
    PatternMatch::initBreakoutAboveFirstHighBreakoutInfo(cup);
}

void CupWithoutHandlePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    visitor.visitCupWithoutHandleStart(*this);
    cup_->acceptVisitor(visitor);
    visitor.visitCupWithoutHandlePatternMatch(*this);
}
