#ifndef PATTERNMATCHBREAKOUTINFO_H
#define PATTERNMATCHBREAKOUTINFO_H

#include <boost/shared_ptr.hpp>
#include "XYCoord.h"

class PatternMatchBreakoutInfo
{
private:
    double periodValPseudoXVal_;
    double breakoutPrice_;
public:
    PatternMatchBreakoutInfo(double pseudoXVal, double breakoutPrice);

    double breakoutPrice() const { return breakoutPrice_; }
    double pseudoXVal() const { return periodValPseudoXVal_; }

    XYCoord xyCoord() const { return XYCoord(periodValPseudoXVal_,breakoutPrice_); }

    virtual ~PatternMatchBreakoutInfo() {}
};

typedef boost::shared_ptr<PatternMatchBreakoutInfo> PatternMatchBreakoutInfoPtr;

#endif // PATTERNMATCHBREAKOUTINFO_H
