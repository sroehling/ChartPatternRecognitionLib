#ifndef CUPWITHHANDLESCANNER_H
#define CUPWITHHANDLESCANNER_H

#include "PatternScanner.h"

class CupWithHandleScanner : public PatternScanner
{
public:
    CupWithHandleScanner();
    PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

};

#endif // CUPWITHHANDLESCANNER_H
