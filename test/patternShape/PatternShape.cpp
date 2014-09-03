#include <boost/test/unit_test.hpp>

#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "TestHelper.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "PatternShape.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;


BOOST_AUTO_TEST_CASE( PatternShape_DoubleBottom_QCOR_20130819 )
{
    // Test a full double-bottom scan for QCOR in 2013. This unit test is built up from
    // VScanner_QCOR_20130819_LHSofDoubleBottom and VScanner_QCOR_20130819_RHSofDoubleBottom, which
    // test the LHS and RHS of the double-bottom individually. The number of pattern matches
    // should therefore be the number of matches from VScanner_QCOR_20130819_LHSofDoubleBottom
    // multiplied times the number of matches from VScanner_QCOR_20130819_RHSofDoubleBottom.

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
    genPeriodValSegmentInfo("Double bottom segment data",*chartData);

    DoubleBottomScanner scanner(DoubleRange(7.0,40.0)); // allow for a deeper depth than the default
    PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

    verifyMatchList("Double bottom match",patternMatches,1);

    verifyPatternMatch("Double bottom match",
            ptime(date(2013,8,26)),ptime(date(2014,2,10)),4,patternMatches->front());

    PatternMatchPtr doubleBottomMatch = patternMatches->front();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*doubleBottomMatch);
    BOOST_CHECK_EQUAL(patternShape->numCurveShapes(),1);

    PatternShapePointVectorVectorPtr curveShapes = patternShape->curveShapes();
    PatternShapePointVectorPtr firstShape = (*curveShapes)[0];

    BOOST_TEST_MESSAGE("PatternShape_DoubleBottom_QCOR_20130819: num points in shape: " << firstShape->size());
    BOOST_CHECK_EQUAL(firstShape->size(),5);

}

BOOST_AUTO_TEST_CASE( PatternShape_DoubleBottom_MultiScan_QCOR_2013_2014_Weekly )
{

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternShape/QCOR_2013_2014_Weekly.csv");

    PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
    MultiPatternScanner multiScanner(doubleBottomScanner);

    PatternMatchListPtr doubleBottoms = multiScanner.scanPatternMatches(chartData);

    // TODO - The scanning seems to be working fine, but the actual pattern matches for a double-bottom don't appear to
    // be correct. In particular, the start dates for some of the matches occur below the RHS of the first dip in the
    // double bottom.
    verifyMatchList("PatternShape_DoubleBottom_MultiScan_QCOR_2013_2014_Weekly: unfiltered matches",doubleBottoms,2);

    PatternMatchListPtr  uniqueMatches = patternMatchFilter::filterUniqueLongestPatternSameEndDate(doubleBottoms);

    genPatternMatchListInfo("Unique matches",*uniqueMatches);

    verifyMatchList("PatternShape_DoubleBottom_MultiScan_QCOR_2013_2014_Weekly: filtered matches",uniqueMatches,1);
    verifyPatternMatch("PatternShape_DoubleBottom_MultiScan_QCOR_2013_2014_Weekly match",
            ptime(date(2013,8,19)),ptime(date(2014,2,10)),4,uniqueMatches,0);



}

