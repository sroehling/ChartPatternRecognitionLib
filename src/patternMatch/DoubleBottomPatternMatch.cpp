#include "DoubleBottomPatternMatch.h"
#include "PatternMatchVisitor.h"
#include "PatternMatchBreakoutInfo.h"

DoubleBottomPatternMatch::DoubleBottomPatternMatch(const PatternMatchPtr &lhsV, const PatternMatchPtr &rhsV)
    : PatternMatch(lhsV->appendMatch(*rhsV)->segments()),
      lhsV_(lhsV),rhsV_(rhsV)
{

    PeriodVal rhsFirstVal = rhsV->firstValue();
    double breakoutThreshold = rhsFirstVal.high();

    PeriodVal rhsLastVal = rhsV->lastValue();
    PeriodVal rhsPreBreakoutVal = rhsV->secondToLastValue();

    double breakoutPseudoX = rhsLastVal.pseudoXVal();

    // If the pattern includes a breakout above the middle of the center V, include the breakout information
    // so it can be plotted. Otherwise, the pattern match is likely a partial match which can be plotted, but doesn't
    // include a breakout.
    //
    // TODO - Somehow support plotting of the partial breakout, where the threshold is known but the breakout hasn't
    // occured yet.
    if((rhsPreBreakoutVal.close() <= breakoutThreshold) &&
            (rhsLastVal.close() > breakoutThreshold))
    {
        this->breakoutInfo = PatternMatchBreakoutInfoPtr(new PatternMatchBreakoutInfo(breakoutPseudoX,breakoutThreshold));
    }

}


void DoubleBottomPatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    lhsV_->acceptVisitor(visitor);
    rhsV_->acceptVisitor(visitor);
    visitor.visitDoubleBottomPatternMatch(*this);
}
