#include "LinearEquation.h"


LinearEquation::LinearEquation(double m, double b)
: m_(m), b_(b)
{
}

double LinearEquation::slope()
{
    return m_;
}

double LinearEquation::yVal(double x)
{
    return m_ * x + b_;
}