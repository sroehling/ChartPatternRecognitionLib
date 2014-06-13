#include "LinearEquation.h"


LinearEquation::LinearEquation(double m, double b)
: m_(m), b_(b)
{
}

LinearEquation::LinearEquation(const XYCoord &startPt, const XYCoord &endPt)
{
	assert(endPt.x() != 0.0); // would cause a divide by zero error.

	b_ = startPt.y();

	// y = mx + b
	// m = (y-b)/x
	m_ = (endPt.y() - b_)/endPt.x();
}

double LinearEquation::slope() const
{
    return m_;
}

double LinearEquation::yVal(double x) const
{
    return m_ * x + b_;
}
