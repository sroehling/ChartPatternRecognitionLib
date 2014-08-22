#ifndef PATTERNSLOPEWITHINRANGE_H
#define PATTERNSLOPEWITHINRANGE_H


#include "PatternMatchValidator.h"
#include "DoubleRange.h"

class PatternSlopeWithinRange : public PatternMatchValidator
{
private:
    DoubleRange slopeRange_;
public:
    PatternSlopeWithinRange(const DoubleRange &slopeRange);
    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // PATTERNSLOPEWITHINRANGE_H
