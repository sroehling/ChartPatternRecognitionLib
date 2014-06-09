/*
 * LinearEquation.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#ifndef LINEAREQUATION_H_
#define LINEAREQUATION_H_


class LinearEquation {

private:
    double m_;
    double b_;
public:
    LinearEquation(double m, double b);
    double slope();
    double yVal(double x);
};


#endif /* LINEAREQUATION_H_ */
