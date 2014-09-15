#ifndef ASCENDINGTRIANGLESCANNER_H
#define ASCENDINGTRIANGLESCANNER_H

#include "TriangleScanner.h"

class AscendingTriangleScanner : public TriangleScanner
{
protected:

    // Test if a completed pattern match is found at the location pointed to by
    // currPerValIter.
    virtual PatternMatchPtr findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    AscendingTriangleScanner();
};

#endif // ASCENDINGTRIANGLESCANNER_H
