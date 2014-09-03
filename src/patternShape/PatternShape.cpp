#include "PatternShape.h"

PatternShape::PatternShape()
{
    curveShapes_ = PatternShapePointVectorVectorPtr(new PatternShapePointVectorVector());
    lineShapes_ = PatternShapePointVectorVectorPtr(new PatternShapePointVectorVector());
}

void PatternShape::addCurveShape(const PatternShapePointVectorPtr &curveShape)
{
    curveShapes_->push_back(curveShape);
}

void PatternShape::addLineShape(const PatternShapePointVectorPtr &lineShape)
{
    lineShapes_->push_back(lineShape);
}
