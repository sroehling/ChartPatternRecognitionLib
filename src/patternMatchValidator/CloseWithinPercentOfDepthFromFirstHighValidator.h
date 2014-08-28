#ifndef CLOSEWITHINPERCENTOFDEPTHFROMFIRSTHIGHVALIDATOR_H
#define CLOSEWITHINPERCENTOFDEPTHFROMFIRSTHIGHVALIDATOR_H

#include "PatternMatchValidator.h"

class CloseWithinPercentOfDepthFromFirstHighValidator : public PatternMatchValidator
{
private:
    double percDepthThreshold_;
public:
    CloseWithinPercentOfDepthFromFirstHighValidator(double percDepthThreshold);
    virtual bool validPattern(const PatternMatch &candidateMatch);
};

#endif // CLOSEWITHINPERCENTOFDEPTHFROMFIRSTHIGHVALIDATOR_H
