#ifndef PERCENTINTERSECTINGPATTERNLINEVALIDATOR_H
#define PERCENTINTERSECTINGPATTERNLINEVALIDATOR_H

#include "PatternMatchValidator.h"
#include "PatternMatch.h"

class PercentIntersectingPatternLineValidator : public PatternMatchValidator
{
public:
    PercentIntersectingPatternLineValidator();
    bool validPattern(const PatternMatch &candidateMatch);
};

#endif // PERCENTINTERSECTINGPATTERNLINEVALIDATOR_H
