#ifndef PATTERNMATCHBREAKOUTINFO_H
#define PATTERNMATCHBREAKOUTINFO_H

#include <boost/shared_ptr.hpp>

class PatternMatchBreakoutInfo
{
private:
    double periodValPseudoXVal_;
    double breakoutPrice_;
public:
    PatternMatchBreakoutInfo(double pseudoXVal, double breakoutPrice);

    double breakoutPrice() const { return breakoutPrice_; }
    double pseudoXVal() const { return periodValPseudoXVal_; }

    virtual ~PatternMatchBreakoutInfo() {}
};

typedef boost::shared_ptr<PatternMatchBreakoutInfo> PatternMatchBreakoutInfoPtr;

#endif // PATTERNMATCHBREAKOUTINFO_H
