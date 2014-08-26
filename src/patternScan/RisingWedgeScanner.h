#ifndef RISINGWEDGESCANNER_H
#define RISINGWEDGESCANNER_H

#include "WedgeScanner.h"

class RisingWedgeScanner : public WedgeScanner
{
protected:

    // Test if a completed pattern match is found at the location pointed to by
    // currPerValIter.
    virtual PatternMatchPtr findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    RisingWedgeScanner();
};

#endif // RISINGWEDGESCANNER_H
