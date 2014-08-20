#ifndef LOWERHIGHPATTERNMATCHVALIDATORFACTORY_H
#define LOWERHIGHPATTERNMATCHVALIDATORFACTORY_H

#include "PatternMatchValidatorFactory.h"

class LowerHighPatternMatchValidatorFactory : public PatternMatchValidatorFactory
{
public:
    LowerHighPatternMatchValidatorFactory();
    virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;
};

#endif // LOWERHIGHPATTERNMATCHVALIDATORFACTORY_H
