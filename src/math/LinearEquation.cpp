#include "LinearEquation.h"


LinearEquation::LinearEquation(double m, double b)
: m_(m), b_(b)
{
}

LinearEquation::LinearEquation(const XYCoord &startPt, const XYCoord &endPt)
{
	double changeX = endPt.x()-startPt.x();
	double changeY = endPt.y()-startPt.y();
	assert(changeX != 0.0);

	m_ = changeY/changeX;

	// y = mx + b
	// b = y - mx
	b_ = endPt.y()-(m_*endPt.x());
}

double LinearEquation::slope() const
{
    return m_;
}

double LinearEquation::yVal(double x) const
{
    return m_ * x + b_;
}
