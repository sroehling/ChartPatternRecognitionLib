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

XYCoord LinearEquation::intercept(const LinearEquation &otherEquation) const
{
	assert(slope() != otherEquation.slope()); // equations never intersect

	// Start with 2 equations
	// y1 = m1 * x1 + b1
	// y2 = m2 * x2 + b2
	//
	// At the point of intersection, we know that y1 = y2
	// and x1 = x2
	//
	// Therefore (at the point of intersection)
	//
	// m1 * x + b1 = m2 * x * b2
	//    or:
	// x = (b2-b1)/(m1-m2)

	double intersectX = (otherEquation.b() - this->b())/(this->slope()-otherEquation.slope());
	double intersectY = this->yVal(intersectX);

	return XYCoord(intersectX, intersectY);

}
