#ifndef HIGHESTHIGHLESSTHANFIRSTHIGH_H
#define HIGHESTHIGHLESSTHANFIRSTHIGH_H

#include "PatternMatchValidator.h"

class HighestHighLessThanFirstHigh : public PatternMatchValidator
{
public:
    HighestHighLessThanFirstHigh();
    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // HIGHESTHIGHLESSTHANFIRSTHIGH_H
