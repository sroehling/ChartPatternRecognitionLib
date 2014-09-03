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

    const PatternMatchPtr &downTrend() const { return downTrend_; }
    const PatternMatchPtr &upTrend() const { return upTrend_; }

};

#endif // VPATTERNMATCH_H
