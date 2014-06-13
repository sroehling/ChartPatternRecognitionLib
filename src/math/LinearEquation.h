/*
 * LinearEquation.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#ifndef LINEAREQUATION_H_
#define LINEAREQUATION_H_

#include <boost/shared_ptr.hpp>
#include "XYCoord.h"

class LinearEquation {

private:
    double m_;
    double b_;
public:
    LinearEquation(double m, double b);
    LinearEquation(const XYCoord &startPt, const XYCoord &endPt);

    double slope() const;
    double yVal(double x) const;
};

typedef boost::shared_ptr<LinearEquation> LinearEquationPtr;

#endif /* LINEAREQUATION_H_ */
