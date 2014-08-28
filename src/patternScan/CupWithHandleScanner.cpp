#include "CupWithHandleScanner.h"
#include "CupScanner.h"
#include "CupWithHandlePatternMatch.h"
#include <boost/log/trivial.hpp>
#include "VScanner.h"

#include "SingleSegmentPatternScannerEngine.h"
#include "PatternSlopeWithinRange.h"
#include "TrendLineScanner.h"


#define DEFAULT_CUP_WITH_HANDLE_SCANNER_MIN_SEGMENT_LENGTH 2
#define DEFAULT_CUP_WITH_HANDLE_SCANNER_HANDLE_MAX_PERC_DISTANCE_TRENDLINE 3.0

CupWithHandleScanner::CupWithHandleScanner()
{
}


PatternMatchListPtr CupWithHandleScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr cupWithHandleMatches(new PatternMatchList());

    CupScanner lhsCupScanner;
    PatternMatchListPtr cupMatches = lhsCupScanner.scanPatternMatches(chartVals);

    BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of cup matches: " << cupMatches->size();


    for(PatternMatchList::const_iterator cupMatchIter = cupMatches->begin();
            cupMatchIter!=cupMatches->end();cupMatchIter++)
    {
        PeriodValSegmentPtr valsForHandleScan = (*cupMatchIter)->trailingValsWithLastVal();

        unsigned int minHandleSegmentLength = DEFAULT_CUP_WITH_HANDLE_SCANNER_MIN_SEGMENT_LENGTH;
        bool validateWithTrendLineValidator = false;

        CupScanner uShapedHandleScanner(minHandleSegmentLength,validateWithTrendLineValidator);
        PatternMatchListPtr uShapedHandleMatches = uShapedHandleScanner.scanPatternMatches(valsForHandleScan);
        BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of U shaped handle matches: " << uShapedHandleMatches->size();

        for(PatternMatchList::const_iterator uShapedHandleMatchIter = uShapedHandleMatches->begin();
            uShapedHandleMatchIter != uShapedHandleMatches->end(); uShapedHandleMatchIter++)
        {
            cupWithHandleMatches->push_back(
                        PatternMatchPtr(new CupWithHandlePatternMatch(*cupMatchIter,*uShapedHandleMatchIter)));
        } // for each u-shaped handle match


        VScanner vShapedHandleScanner(DEFAULT_CUP_WITH_HANDLE_SCANNER_HANDLE_MAX_PERC_DISTANCE_TRENDLINE,
                                      minHandleSegmentLength,validateWithTrendLineValidator);
        PatternMatchListPtr vShapedHandleMatches = vShapedHandleScanner.scanPatternMatches(valsForHandleScan);
        BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of V shaped handle matches: " << vShapedHandleMatches->size();

        for(PatternMatchList::const_iterator vShapedHandleMatchIter = vShapedHandleMatches->begin();
            vShapedHandleMatchIter != vShapedHandleMatches->end(); vShapedHandleMatchIter++)
        {
            cupWithHandleMatches->push_back(
                        PatternMatchPtr(new CupWithHandlePatternMatch(*cupMatchIter,*vShapedHandleMatchIter)));
        } // for each v-shaped handle match

    }

    return cupWithHandleMatches;
}
