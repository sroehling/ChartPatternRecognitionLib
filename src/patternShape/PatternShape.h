#ifndef PATTERNSHAPE_H_
#define PATTERNSHAPE_H_

#include "PeriodVal.h"
#include "PatternShapePoint.h"
#include <boost/shared_ptr.hpp>


class PatternShape {
private:
    PatternShapePointVectorVectorPtr curveShapes_;
    PatternShapePointVectorVectorPtr lineShapes_;
public:
    PatternShape();

    // Add a vector of PatternShapePoint's comprising a curve or line
    // for the pattern shape.
    void addCurveShape(const PatternShapePointVectorPtr &curveShape);
    void addLineShape(const PatternShapePointVectorPtr &lineShape);


    unsigned int numCurveShapes() const { return curveShapes_->size(); }
    PatternShapePointVectorVectorPtr curveShapes() const { return curveShapes_; }

    unsigned int numLineShapes() const { return lineShapes_->size(); }
    PatternShapePointVectorVectorPtr lineShapes() const { return lineShapes_; }

    unsigned int numShapes() const { return numLineShapes() + numCurveShapes(); }


    virtual ~PatternShape() {}
};

typedef boost::shared_ptr<PatternShape> PatternShapePtr;

#endif
