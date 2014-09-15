#ifndef DESCEDNDINGTRIANGLESCANNER_H
#define DESCEDNDINGTRIANGLESCANNER_H

#include "TriangleScanner.h"

class DescendingTriangleScanner : public TriangleScanner
{
protected:

    // Test if a completed pattern match is found at the location pointed to by
    // currPerValIter.
    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    DescendingTriangleScanner();
};

#endif // FALLINGWEDGESCANNER_H
