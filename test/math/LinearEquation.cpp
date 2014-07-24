
#include <boost/test/unit_test.hpp>

#include "LinearEquation.h"

BOOST_AUTO_TEST_CASE( LinearEquationTest )
{
    LinearEquation noSlope(0.0,0.0);

    BOOST_CHECK( noSlope.yVal(0.0) == 0.0);
}


BOOST_AUTO_TEST_CASE( LinearEquation_Intersection )
{

	// Example taken from: https://www.evernote.com/shard/s3/sh/da1d5daa-2e24-404a-b26c-f84d9edb1aa3/4c4e4dd48d11085f49629933e0600aa2
    LinearEquation equation1(3.0,2.0);
    LinearEquation equation2(2.0,-1.0);

    XYCoord intercept = equation1.intercept(equation2);

    BOOST_CHECK( intercept.x() == -3.0);
    BOOST_CHECK( intercept.y() == -7.0);
}
