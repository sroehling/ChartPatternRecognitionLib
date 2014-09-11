/*
 * XYCoord.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#ifndef XYCOORD_H_
#define XYCOORD_H_

#include <ostream>
#include <vector>

class XYCoord {
private:
    double x_;
    double y_;

public:
    XYCoord(double x, double y);

    double x() const;
    double y() const;

	friend std::ostream& operator<<(std::ostream& os, const XYCoord& coord);

};

typedef std::vector<XYCoord> XYCoordVector;


#endif /* XYCOORD_H_ */
