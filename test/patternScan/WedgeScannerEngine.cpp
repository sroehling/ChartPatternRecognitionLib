#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricWedgeScanner.h"
#include "PatternShapeGenerator.h"
#include "FallingWedgeScanner.h"
#include "MultiPatternScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( WedgeScannerEngine_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    PatternScannerPtr scanner(new SymetricWedgeScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    // TODO - The scanning seems to be working fine, but the actual pattern matches for a double-bottom don't appear to
    // be correct. In particular, the start dates for some of the matches occur below the RHS of the first dip in the
    // double bottom.
  //  verifyMatchList("WedgeScannerEngine_VZ_SymetricTriangle: unfiltered matches",doubleBottoms,5);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("WedgeScannerEngine_VZ_SymetricTriangle: filtered matches",symetricTriangles,1);


    PatternMatchPtr wedgeMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("WedgeScannerEngine_SynthesizedPattern match",
            ptime(date(2013,7,29)),ptime(date(2013,10,7)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( WedgeScannerEngine_GLD_Wedges )
{
    // The chart data from 2013 to 2014 contains a number of potential wedge matches.

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GLD_Weekly_2013_2014.csv");

    PatternScannerPtr scanner(new SymetricWedgeScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);
    verifyMatchList("WedgeScannerEngine_VZ_SymetricTriangle: filtered matches",symetricTriangles,0);


    FallingWedgeScanner fallingWedgeScanner;
    PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData);
    verifyMatchList("WedgeScannerEngine_VZ_SymetricTriangle: falling wedges",fallingWedges,0);

}



BOOST_AUTO_TEST_CASE( WedgeScannerEngine_CELG_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CELG_20140501_20140814_Daily.csv");

    PatternScannerPtr scanner(new SymetricWedgeScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    genPatternMatchListInfo("Unique matches",*symetricTriangles);

    verifyMatchList("WedgeScannerEngine_CELG_SymetricTriangle: filtered matches",symetricTriangles,1);


    PatternMatchPtr wedgeMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);

    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();

    verifyPatternMatch("WedgeScannerEngine_CELG_SymetricTriangle match",
            ptime(date(2014,7,3)),ptime(date(2014,8,13)),1,symetricTriangles,0);


}


BOOST_AUTO_TEST_CASE( WedgeScannerEngine_GMCR_FallingWedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GMCR_Daily_2014.csv");

    PatternScannerPtr scanner(new SymetricWedgeScanner());
    PatternMatchListPtr symetricTriangles = scanner->scanPatternMatches(chartData);

    FallingWedgeScanner fallingWedgeScanner;
    PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData);

    // TODO - The falling wedge scanner is not currently removing duplicates. When it does, there
    // should only be 1 instead of 3 falling wedges returned from the scanner.
    verifyMatchList("WedgeScannerEngine_GMCR_FallingWedge: falling wedges",fallingWedges,3);
    verifyPatternMatch("WedgeScannerEngine_CELG_SymetricTriangle falling wedge match",
            ptime(date(2014,5,13)),ptime(date(2014,6,6)),1,fallingWedges,0);

    PatternMatchPtr wedgeMatch = fallingWedges->back();
    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numLineShapes(),2);
    PatternShapePointVectorVectorPtr lineShapes = patternShape->lineShapes();


}

