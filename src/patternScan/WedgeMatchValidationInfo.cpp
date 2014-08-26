#include "WedgeMatchValidationInfo.h"

WedgeMatchValidationInfo::WedgeMatchValidationInfo(const PeriodValSegmentPtr &chartVals,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine,
        const PeriodValCltn::iterator &currPerValIter)
: chartVals_(chartVals),
  upperTrendLine_(upperTrendLine),
  lowerTrendLine_(lowerTrendLine),
  currPerValIter_(currPerValIter)
{
}

const PeriodValCltn::iterator WedgeMatchValidationInfo::patternBeginIter() const
{
    return upperTrendLine_->firstValIter();
}


const PeriodValCltn::iterator WedgeMatchValidationInfo::patternEndIter() const
{
    PeriodValCltn::iterator endIter = currPerValIter_;
    assert(endIter != chartVals_->segEnd());
    endIter++;
    return endIter;
}
