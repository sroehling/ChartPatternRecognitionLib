
#include <boost/log/trivial.hpp>

#include "CupWithoutHandleScanner.h"
#include "CloseWithinPercentOfDepthFromFirstHighValidator.h"
#include "BreakoutAboveFirstHighValidatorFactory.h"
#include "CupScanner.h"
#include "CupWithoutHandlePatternMatch.h"

#define CUP_WITHOUT_HANDLE_SCANNER_CUP_LAST_CLOSE_PERC_ABOVE_DEPTH_FROM_FIRST_CLOSE_THRESHOLD 0.2


CupWithoutHandleScanner::CupWithoutHandleScanner()
{
}


PatternMatchListPtr CupWithoutHandleScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr cupWithHandleMatches(new PatternMatchList());

    CupScanner cupScanner;
    cupScanner.overallValidatorFactory().addFactory(
                PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));

    PatternMatchListPtr cupMatches = cupScanner.scanPatternMatches(chartVals);

    for(PatternMatchList::iterator cupIter = cupMatches->begin(); cupIter != cupMatches->end();
        cupIter++)
    {
        cupWithHandleMatches->push_back(PatternMatchPtr(new CupWithoutHandlePatternMatch(*cupIter)));
    }

    BOOST_LOG_TRIVIAL(debug) << "CupWithoutHandleScanner: number of cup matches: " << cupWithHandleMatches->size();

    return cupWithHandleMatches;
}
