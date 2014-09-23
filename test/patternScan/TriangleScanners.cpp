#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricTriangleScanner.h"
#include "PatternShapeGenerator.h"
#include "DescendingTriangleScanner.h"
#include "MultiPatternScanner.h"
#include "PivotHighScanner.h"
#include "PivotLowScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( TriangleScanners_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << chartData->averageDaysPerPeriod() );
    BOOST_CHECK_CLOSE(chartData->averageDaysPerPeriod(),6.9863,0.1);

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

    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << chartData->averageDaysPerPeriod() );
    BOOST_CHECK_CLOSE(chartData->averageDaysPerPeriod(),6.945,0.01);

    PatternScannerPtr scanner(new SymetricTriangleScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);
    verifyMatchList("TriangleScanners_VZ_SymetricTriangle: filtered matches",symetricTriangles,0);


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


