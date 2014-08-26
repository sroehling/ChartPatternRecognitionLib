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

ChartSegmentPtr WedgeMatchValidationInfo::createWedgeSegment() const
{
    ChartSegmentPtr wedgeSeg(new ChartSegment(chartVals->perValCltn(),
            patternBeginIter(),currPerValIter_,
            PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
    return wedgeSeg;
}


bool WedgeMatchValidationInfo::upperTrendLineBreakout() const
{
    PeriodValCltn::iterator prevPerValIter = currPerValIter_;
    prevPerValIter--;
    assert(prevPerValIter != chartVals->segBegin());

    if (upperTrendLine_->segmentEq()->belowLine((*prevPerValIter).closeCoord()) &&
            upperTrendLine_->segmentEq()->aboveLine((*currPerValIter_).closeCoord()))
    {
        BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: upper trend line breakout: "
                    << "prev val=" << (*prevPerValIter).closeCoord()
                    << ", curr val=" << (*currPerValIter_).closeCoord()
                    << ", curr period val=" << (*currPerValIter_) << std::endl;
        return true;
    }
    else
    {
        return false;
    }

}


