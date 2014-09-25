#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricTriangleScanner.h"
#include "PatternShapeGenerator.h"
#include "DescendingTriangleScanner.h"
#include "RisingWedgeScanner.h"
#include "MultiPatternScanner.h"
#include "PivotHighScanner.h"
#include "PivotLowScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( TriangleScanners_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << chartData->averageCalendarDaysPerPeriod() );
    BOOST_CHECK_CLOSE(chartData->averageCalendarDaysPerPeriod(),6.9863,0.1);

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    // TODO - The scanning seems to be working fine, but the actual pattern matches for a double-bottom don't appear to
    // be correct. In particular, the start dates for some of the matches occur below the RHS of the first dip in the
    // double bottom.
  //  verifyMatchList("TriangleScanners_VZ_SymetricTriangle: unfiltered matches",doubleBottoms,5);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: filtered matches",symetricTriangles,2);




    PatternMatchPtr triangleMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*triangleMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("TriangleScanners_SynthesizedPattern match",
            ptime(date(2013,7,29)),ptime(date(2014,5,5)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( TriangleScanners_GLD_Wedges )
{
    // The chart data from 2013 to 2014 contains a number of potential wedge matches.

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GLD_Weekly_2013_2014.csv");

    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << chartData->averageCalendarDaysPerPeriod() );
    BOOST_CHECK_CLOSE(chartData->averageCalendarDaysPerPeriod(),6.945,0.01);


    // The scanning for symetric triangles and rising wedges is unique for this chart data, in that
    // the pattern matches end on the last period of of the chart data. There was an off-by-one-bug
    // related to matching patterns ending on the last period. The 2 patterns matched here
    // were only showing intermittently before fixing this bug (the code was indexing beyond the
    // end of the last period).

    PatternScannerPtr symetricTriangleScanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = symetricTriangleScanner->scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: filtered matches",symetricTriangles,1);

    verifyPatternMatch("TriangleScanners_VZ_SymetricTriangle symetric triangle",
            ptime(date(2013,8,26)),ptime(date(2014,8,18)),1,symetricTriangles,0);

    BOOST_CHECK_EQUAL(symetricTriangles->front()->isConfirmedMatch(),true);


    PatternScannerPtr risingWedgeScanner(new RisingWedgeScanner());
    PatternMatchListPtr risingWedges = risingWedgeScanner->scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: rising wedges",risingWedges,1);

    verifyPatternMatch("TriangleScanners_VZ_SymetricTriangle rising wedge",
            ptime(date(2014,5,5)),ptime(date(2014,8,18)),1,risingWedges,0);

    BOOST_CHECK_EQUAL(risingWedges->front()->isConfirmedMatch(),true);


    DescendingTriangleScanner descTriangleScanner;
    PatternMatchListPtr descTriangles = descTriangleScanner.scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: descending triangles",descTriangles,0);

}



BOOST_AUTO_TEST_CASE( TriangleScanners_CELG_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CELG_20140501_20140814_Daily.csv");

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("TriangleScanners_CELG_SymetricTriangle: filtered matches",symetricTriangles,1);


    PatternMatchPtr wedgeMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("TriangleScanners_CELG_SymetricTriangle match",
            ptime(date(2014,7,3)),ptime(date(2014,8,13)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( TriangleScanners_GMCR_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GMCR_Daily_2014.csv");

    PatternMatchListPtr pivotHighs = PivotHighScanner().scanPatternMatches(chartData);
    PatternMatchListPtr pivotLows = PivotLowScanner().scanPatternMatches(chartData);

    genPivotHighInfo(pivotHighs);
    genPivotLowInfo(pivotLows);


    DescendingTriangleScanner descendingTriangleScanner;
    PatternMatchListPtr descTriangles = descendingTriangleScanner.scanPatternMatches(chartData);

    verifyMatchList("TriangleScanners_GMCR_Wedge: descending triangles",descTriangles,0);

}

BOOST_AUTO_TEST_CASE( TriangleScanners_CMG_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CMG_Daily.csv");

    PatternScannerPtr symetricScanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = symetricScanner->scanPatternMatches(chartData);

    // Should match 0 patterns, since the patterns have the pivots spaced out quite a bit, leaving
    // a lot of "empty space" in the middle. However, the test WedgeScanners_CMG_Falling_Wedge
    // finds a well-formed falling wedge.
    verifyMatchList("TriangleScanners_GMCR_Wedge: symetric triangles",symetricTriangles,0);

}


