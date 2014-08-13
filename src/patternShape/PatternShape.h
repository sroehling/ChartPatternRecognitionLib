#ifndef PATTERNSHAPE_H_
#define PATTERNSHAPE_H_

#include "PeriodVal.h"
#include "PatternShapePoint.h"
#include <boost/shared_ptr.hpp>


class PatternShape {
private:
    PatternShapePointVectorVectorPtr curveShapes_;
public:
    PatternShape();

    // Add a vector of PatternShapePoint's comprising a curve or line
    // for the pattern shape.
    void addCurveShape(const PatternShapePointVectorPtr &curveShape);

    unsigned int numCurveShapes() const { return curveShapes_->size(); }
    PatternShapePointVectorVectorPtr curveShapes() const { return curveShapes_; }

    virtual ~PatternShape() {}
};

typedef boost::shared_ptr<PatternShape> PatternShapePtr;

#endif
