#ifndef PATTERNSHAPECURVEGENERATOR_H
#define PATTERNSHAPECURVEGENERATOR_H

#include "PatternShapePoint.h"
#include "XYCoord.h"

class PatternShapeCurveGenerator
{
public:
    PatternShapeCurveGenerator() {}

    PatternShapePointVectorPtr createCurvedShape(const XYCoordVector &shapePoints);

    virtual ~PatternShapeCurveGenerator() {}
};

#endif // PATTERNSHAPECURVEGENERATOR_H
