#ifndef PATTERNSHAPE_H_
#define PATTERNSHAPE_H_

#include "PeriodVal.h"
#include "PatternShapePoint.h"

class PatternShape {
private:
    PatternShapePointVector shapePoints_; // points in the shape, from start to finish
public:
    PatternShape(const PatternShapePointVector &shapePoints);
    void addShapePoint(const PeriodVal &shapePt);
};



#endif
