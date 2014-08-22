#ifndef SINGLESEGMENTPATTERNSCANNERENGINE_H
#define SINGLESEGMENTPATTERNSCANNERENGINE_H

#include "UnsignedIntRange.h"
#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternMatchValidatorFactory.h"
#include "CompositePatternMatchValidatorFactory.h"

class SingleSegmentPatternScannerEngine
{
private:
    UnsignedIntRange segmentLengthRange_;

    CompositePatternMatchValidatorFactory validatorFactory_;


public:
    SingleSegmentPatternScannerEngine();

    static const UnsignedIntRange DEFAULT_SCAN_LENGTH_RANGE;

    PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

    CompositePatternMatchValidatorFactory &validatorFactory() { return validatorFactory_; }



};

#endif // SINGLESEGMENTPATTERNSCANNERENGINE_H
