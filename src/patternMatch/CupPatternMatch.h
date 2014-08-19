#ifndef CUPPATTERNMATCH_H
#define CUPPATTERNMATCH_H

#include "PatternMatch.h"

class CupPatternMatch : public PatternMatch
{
private:
    PatternMatchPtr downTrend_;
    PatternMatchPtr cupBottom_;
    PatternMatchPtr upTrend_;
public:
    CupPatternMatch(const PatternMatchPtr &downTrend, const PatternMatchPtr &cupBottom,
                    const PatternMatchPtr &upTrend);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    const PatternMatchPtr &downTrend() const { return downTrend_; }
    const PatternMatchPtr &cupBottom() const { return cupBottom_; }
    const PatternMatchPtr &upTrend() const { return upTrend_; }

    virtual std::string matchType() const { return "Cup"; }

};

#endif // CUPPATTERNMATCH_H
