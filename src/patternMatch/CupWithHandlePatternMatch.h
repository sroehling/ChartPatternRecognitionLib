#ifndef CUPWITHHANDLEPATTERNMATCH_H
#define CUPWITHHANDLEPATTERNMATCH_H

#include "PatternMatch.h"

class CupWithHandlePatternMatch : public PatternMatch
{
private:
    PatternMatchPtr cup_;
    PatternMatchPtr handle_;
public:
    CupWithHandlePatternMatch(const PatternMatchPtr &cup, const PatternMatchPtr &handle);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Cup with Handle"; }

};

#endif // CUPWITHHANDLEPATTERNMATCH_H
