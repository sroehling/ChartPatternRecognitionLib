#ifndef RISINGWEDGEPATTERNMATCH_H
#define RISINGWEDGEPATTERNMATCH_H

#include "WedgePatternMatch.h"

class RisingWedgePatternMatch : public WedgePatternMatch
{
public:
    RisingWedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo);

    virtual std::string matchType() const { return "Rising Wedge"; }

};

#endif // RISINGWEDGEPATTERNMATCH_H
