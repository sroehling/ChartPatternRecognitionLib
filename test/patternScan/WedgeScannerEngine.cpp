#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricWedgeScanner.h"
#include "PatternShapeGenerator.h"
#include "MultiPatternScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( WedgeScannerEngine_SynthesizedPattern )
{
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,92.0,100.0)); // up-trend to first pivot high at 100
	ranges.push_back(TestPerValRange(4,98.0,92.0)); // down-trend to first pivot low at 92
	ranges.push_back(TestPerValRange(4,92.5,98.0)); // up-trend to 2nd pivot high at 98 (lower than 1st)
	ranges.push_back(TestPerValRange(4,97.5,94.0)); // down-trend to 2nd pivot low at 94 (higher than 1st)
	ranges.push_back(TestPerValRange(4,94.5,104.0)); // up-trend after 2nd pivot low, including up-side break-out
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	SymetricWedgeScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("Should return 1 match, the break-out after the 2nd pivot low");
	verifyMatchList("WedgeScannerEngine_SynthesizedPattern",patternMatches,1);

	verifyPatternMatch("WedgeScannerEngine_SynthesizedPattern match",
			ptime(date(2014,1,4)),ptime(date(2014,1,18)),1,patternMatches,0);
}


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

    verifyMatchList("WedgeScannerEngine_VZ_SymetricTriangle: filtered matches",symetricTriangles,17);


    PatternMatchPtr wedgeMatch = symetricTriangles->back();

    PatternShapeGenerator shapeGen;
    PatternShapePtr patternShape = shapeGen.generateShape(*wedgeMatch);
    BOOST_CHECK_EQUAL(patternShape->numCurveShapes(),2);

    PatternShapePointVectorVectorPtr curveShapes = patternShape->curveShapes();

    verifyPatternMatch("WedgeScannerEngine_SynthesizedPattern match",
            ptime(date(2013,4,29)),ptime(date(2013,10,14)),1,symetricTriangles,0);


}
