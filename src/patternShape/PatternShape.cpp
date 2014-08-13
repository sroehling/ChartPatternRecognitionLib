#include "PatternShape.h"

PatternShape::PatternShape()
{
    curveShapes_ = PatternShapePointVectorVectorPtr(new PatternShapePointVectorVector());
}

void PatternShape::addCurveShape(const PatternShapePointVectorPtr &curveShape)
{
    curveShapes_->push_back(curveShape);
}
