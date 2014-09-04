#ifndef CUPWITHOUTHANDLEPATTERNMATCH_H
#define CUPWITHOUTHANDLEPATTERNMATCH_H

#include "PatternMatch.h"

class CupWithoutHandlePatternMatch : public PatternMatch
{
private:
    PatternMatchPtr cup_;
public:
    CupWithoutHandlePatternMatch(const PatternMatchPtr &cup);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Cup without Handle"; }

};


#endif // CUPWITHOUTHANDLEPATTERNMATCH_H
