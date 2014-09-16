#ifndef FALLINGWEDGESCANNER_H
#define FALLINGWEDGESCANNER_H

#include "WedgeScanner.h"

class FallingWedgeScanner: public WedgeScanner
{
public:
    FallingWedgeScanner();

    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;

};

#endif // FALLINGWEDGESCANNER_H
