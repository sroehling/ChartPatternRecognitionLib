#ifndef PREVPATTERNVALUERATIOVALIDATORFACTORY_H
#define PREVPATTERNVALUERATIOVALIDATORFACTORY_H

#include "PatternMatchValidatorFactory.h"
#include "DoubleRange.h"
#include "PatternMatchValueRef.h"

class PrevPatternValueRatioValidatorFactory : public PatternMatchValidatorFactory
{
private:
    DoubleRange validRatios_;
    PatternMatchValueRefPtr ratioValueRef_; // value reference for ratio values
public:
    PrevPatternValueRatioValidatorFactory(const DoubleRange &validRatios,
                                          const PatternMatchValueRefPtr &ratioValueRef);
    virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;
};

#endif // PREVPATTERNVALUERATIOVALIDATORFACTORY_H
