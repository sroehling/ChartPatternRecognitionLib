#ifndef DOUBLEBOTTOMPATTERNMATCH_H
#define DOUBLEBOTTOMPATTERNMATCH_H

#include "PatternMatch.h"

class DoubleBottomPatternMatch : public PatternMatch
{
private:
    PatternMatchPtr lhsV_;
    PatternMatchPtr rhsV_;
public:
    DoubleBottomPatternMatch(const PatternMatchPtr &lhsV, const PatternMatchPtr &rhsV);
    virtual void acceptVisitor(PatternMatchVisitor &visitor);
};

#endif // DOUBLEBOTTOMPATTERNMATCH_H
