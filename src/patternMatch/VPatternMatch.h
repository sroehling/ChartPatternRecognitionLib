#ifndef VPATTERNMATCH_H
#define VPATTERNMATCH_H

#include "PatternMatch.h"
#include "ChartSegment.h"

class VPatternMatch: public PatternMatch {
    PatternMatchPtr downTrend_;
    PatternMatchPtr upTrend_;
public:
    VPatternMatch(const PatternMatchPtr &downTrend, const PatternMatchPtr upTrend);
    virtual void acceptVisitor(PatternMatchVisitor &visitor);
};

#endif // VPATTERNMATCH_H
