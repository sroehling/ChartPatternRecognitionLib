#ifndef CUPWITHHANDLESCANNER_H
#define CUPWITHHANDLESCANNER_H

#include "PatternScanner.h"
#include "CompositePatternMatchValidatorFactory.h"

class CupWithHandleScanner : public PatternScanner
{
private:
    CompositePatternMatchValidatorFactory handleValidatorFactory_;
public:
    CupWithHandleScanner();
    PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

};

#endif // CUPWITHHANDLESCANNER_H
