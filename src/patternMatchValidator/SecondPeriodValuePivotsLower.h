#ifndef SECONDPERIODVALUEPIVOTSLOWER_H
#define SECONDPERIODVALUEPIVOTSLOWER_H

#include "PatternMatchValidator.h"

class SecondPeriodValuePivotsLower : public PatternMatchValidator
{
public:
    SecondPeriodValuePivotsLower();

    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // SECONDPERIODVALUEPIVOTSLOWER_H
