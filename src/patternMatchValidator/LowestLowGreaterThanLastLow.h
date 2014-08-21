#ifndef LOWESTLOWGREATERTHANLASTLOW_H
#define LOWESTLOWGREATERTHANLASTLOW_H

#include "PatternMatch.h"
#include "PatternMatchValidator.h"

class LowestLowGreaterThanLastLow : public PatternMatchValidator
{
public:
    LowestLowGreaterThanLastLow();
    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // LOWESTLOWGREATERTHANLASTLOW_H
