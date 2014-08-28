#ifndef BREAKOUTABOVEFIRSTHIGHVALIDATORFACTORY_H
#define BREAKOUTABOVEFIRSTHIGHVALIDATORFACTORY_H

#include "PatternMatchValidatorFactory.h"

class BreakoutAboveFirstHighValidatorFactory : public PatternMatchValidatorFactory
{
public:
    BreakoutAboveFirstHighValidatorFactory();
    PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;

};

#endif // BREAKOUTABOVEFIRSTHIGHVALIDATORFACTORY_H
