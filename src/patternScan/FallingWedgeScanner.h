#ifndef FALLINGWEDGESCANNER_H
#define FALLINGWEDGESCANNER_H

#include "WedgeScanner.h"

class FallingWedgeScanner : public WedgeScanner
{
protected:

    // Test if a completed pattern match is found at the location pointed to by
    // currPerValIter.
    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    FallingWedgeScanner();
};

#endif // FALLINGWEDGESCANNER_H
