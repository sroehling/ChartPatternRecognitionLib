/*
 * XYCoord.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#ifndef XYCOORD_H_
#define XYCOORD_H_


class XYCoord {
private:
    double x_;
    double y_;

public:
    XYCoord(double x, double y);

    double x() const;
    double y() const;

};


#endif /* XYCOORD_H_ */
