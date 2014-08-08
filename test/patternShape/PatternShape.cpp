#include <boost/test/unit_test.hpp>

#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "DoubleBottomScanner.h"
#include "TestHelper.h"
#include "PatternShapeGenerator.h"

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
            ptime(date(2013,8,26)),ptime(date(2014,2,18)),5,patternMatches->front());

    PatternMatchPtr doubleBottomMatch = patternMatches->front();

    PatternShapeGenerator shapeGen;
    PatternShapePointVectorPtr shapeCurve = shapeGen.generateShape(*doubleBottomMatch);
    BOOST_TEST_MESSAGE("PatternShape_DoubleBottom_QCOR_20130819: num points in shape: " << shapeCurve->size());
    BOOST_CHECK_EQUAL(shapeCurve->size(),5);

}

