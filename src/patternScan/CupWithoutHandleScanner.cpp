

#include "CupWithoutHandleScanner.h"
#include "CloseWithinPercentOfDepthFromFirstHighValidator.h"
#include "BreakoutAboveFirstHighValidatorFactory.h"
#include "CupScanner.h"
#include "CupWithoutHandlePatternMatch.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "DebugLog.h"

#define CUP_WITHOUT_HANDLE_SCANNER_CUP_LAST_CLOSE_PERC_ABOVE_DEPTH_FROM_FIRST_CLOSE_THRESHOLD 0.2

#define CUP_WITHOUT_HANDLE_MIN_DEPTH_PERC_SINCE_START 8.0
#define CUP_WITHOUT_HANDLE_MAX_DEPTH_PERC_SINCE_START 30.0
#define CUP_WITHOUT_HANDLE_MIN_CALENDAR_DAYS_LENGTH 20.0

using namespace patternMatchValidatorCreationHelper;

CupWithoutHandleScanner::CupWithoutHandleScanner()
{
}


PatternMatchListPtr CupWithoutHandleScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr cupWithHandleMatches(new PatternMatchList());

    CupScanner cupScanner;
    cupScanner.overallValidatorFactory().addFactory(
                PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));
    cupScanner.overallValidatorFactory().addStaticValidator(depthSinceStartWithinRangeValidator(
        DoubleRange(CUP_WITHOUT_HANDLE_MIN_DEPTH_PERC_SINCE_START,CUP_WITHOUT_HANDLE_MAX_DEPTH_PERC_SINCE_START)));

    cupScanner.overallValidatorFactory().addStaticValidator(
          minValValidator(CUP_WITHOUT_HANDLE_MIN_CALENDAR_DAYS_LENGTH,
                          PatternMatchValueRefPtr(new CalendarDaysLengthPatternMatchValueRef())));


    PatternMatchListPtr cupMatches = cupScanner.scanPatternMatches(chartVals);

    for(PatternMatchList::iterator cupIter = cupMatches->begin(); cupIter != cupMatches->end();
        cupIter++)
    {
        cupWithHandleMatches->push_back(PatternMatchPtr(new CupWithoutHandlePatternMatch(*cupIter)));
    }

    DEBUG_MSG("CupWithoutHandleScanner: number of cup matches: " << cupWithHandleMatches->size());

    return cupWithHandleMatches;
}
