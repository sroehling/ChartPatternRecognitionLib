
#include <boost/test/unit_test.hpp>

#include "LinearEquation.h"

BOOST_AUTO_TEST_CASE( LinearEquationTest )
{
    LinearEquation noSlope(0.0,0.0);

    BOOST_CHECK( noSlope.yVal(0.0) == 0.0);
}
