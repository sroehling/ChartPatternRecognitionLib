/*
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LinearEquationTest
*/

#include <boost/test/unit_test.hpp>

#include "XYCoord.h"

BOOST_AUTO_TEST_CASE( XYCoordTest )
{
    XYCoord simple(1.0,2.0);

    BOOST_CHECK( simple.x() == 1.0);
    BOOST_CHECK( simple.y() == 2.0);
}
