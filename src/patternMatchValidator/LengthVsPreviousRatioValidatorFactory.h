#ifndef LENGTHVSPREVIOUSRATIOVALIDATORFACTORY_H
#define LENGTHVSPREVIOUSRATIOVALIDATORFACTORY_H

#include "PatternMatchValidatorFactory.h"
#include "PatternMatchValueRef.h"
#include "ValueComparator.h"

class LengthVsPreviousRatioValidatorFactory : public PatternMatchValidatorFactory
{
public:
    LengthVsPreviousRatioValidatorFactory();
    virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;

};

#endif // LENGTHVSPREVIOUSRATIOVALIDATORFACTORY_H
