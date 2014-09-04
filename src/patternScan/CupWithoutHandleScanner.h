#ifndef CUPWITHOUTHANDLESCANNER_H
#define CUPWITHOUTHANDLESCANNER_H

#include "PatternScanner.h"

class CupWithoutHandleScanner : public PatternScanner
{
public:
    CupWithoutHandleScanner();
    PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
};

#endif // CUPWITHOUTHANDLESCANNER_H
