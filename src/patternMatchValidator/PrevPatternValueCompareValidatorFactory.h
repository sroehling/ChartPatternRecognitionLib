#ifndef PREVPATTERNVALUECOMPAREVALIDATORFACTORY_H
#define PREVPATTERNVALUECOMPAREVALIDATORFACTORY_H

#include "PatternMatchValidatorFactory.h"
#include "ValueComparator.h"
#include "PatternMatchValueRef.h"

class PrevPatternValueCompareValidatorFactory : public PatternMatchValidatorFactory
{
private:
    ValueComparatorPtr comparison_;
    PatternMatchValueRefPtr compareValueRef_; // value used for comparison
public:
    PrevPatternValueCompareValidatorFactory(const ValueComparatorPtr &comparison,
            const PatternMatchValueRefPtr &compareValueRef);
    virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;
};

#endif // PREVPATTERNVALUECOMPAREVALIDATORFACTORY_H
