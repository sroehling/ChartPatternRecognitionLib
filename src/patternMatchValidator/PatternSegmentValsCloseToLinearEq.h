#ifndef PATTERNSEGMENTVALSCLOSETOLINEAREQ_H
#define PATTERNSEGMENTVALSCLOSETOLINEAREQ_H

#include "PatternMatch.h"
#include "PatternMatchValidator.h"

class PatternSegmentValsCloseToLinearEq : public PatternMatchValidator
{
private:
    double maxDeltaPercVsEquation_;
public:
    PatternSegmentValsCloseToLinearEq(double maxDeltaPercVsEquation);
    virtual bool validPattern(const PatternMatch &candidateMatch);

};

#endif // PATTERNSEGMENTVALSCLOSETOLINEAREQ_H
