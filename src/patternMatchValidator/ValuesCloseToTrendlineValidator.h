#ifndef VALUESCLOSETOTRENDLINEVALIDATOR_H
#define VALUESCLOSETOTRENDLINEVALIDATOR_H

#include "PatternMatchValidator.h"

class ValuesCloseToTrendlineValidator : public PatternMatchValidator
{
public:
    ValuesCloseToTrendlineValidator();
    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // VALUESCLOSETOTRENDLINEVALIDATOR_H
