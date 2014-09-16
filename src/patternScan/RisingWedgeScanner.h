#ifndef RISINGWEDGESCANNER_H
#define RISINGWEDGESCANNER_H

#include "WedgeScanner.h"

class RisingWedgeScanner : public WedgeScanner
{
public:
    RisingWedgeScanner();

    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;

};

#endif // RISINGWEDGESCANNER_H
