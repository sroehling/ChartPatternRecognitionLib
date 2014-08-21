#ifndef PREVPATTERNDEPTHTHRESHOLD_H
#define PREVPATTERNDEPTHTHRESHOLD_H

#include "PatternMatchValidatorFactory.h"
#include "PatternMatchValueRef.h"
#include "ValueComparator.h"

class PrevPatternDepthThreshold : public PatternMatchValidatorFactory
{
private:
    double depthThresholdMult_;
    PatternMatchValueRefPtr comparisonVal_;
    ValueComparatorPtr comparator_;
public:
    PrevPatternDepthThreshold(double depthThresholdMult,
            const PatternMatchValueRefPtr &comparisonVal,
           const ValueComparatorPtr &comparator);
    virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;
};

#endif // PREVPATTERNDEPTHTHRESHOLD_H
