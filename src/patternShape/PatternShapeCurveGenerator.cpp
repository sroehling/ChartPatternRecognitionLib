#include "PatternShapeCurveGenerator.h"
#include "LinearEquation.h"
#include <assert.h>

#define EXTRAPOLATED_CONTROL_POINT_X_VALUE_OFFSET 2.0

// Catmull–Rom spline algorithm
static XYCoord pointOnCurve(XYCoord p0, XYCoord p1, XYCoord p2, XYCoord p3, double t)
{
    double t2 = t * t;
    double t3 = t2 * t;

    double retX = 0.5f * ((2.0f * p1.x()) +
    (-p0.x() + p2.x()) * t +
    (2.0f * p0.x() - 5.0f * p1.x() + 4 * p2.x() - p3.x()) * t2 +
    (-p0.x() + 3.0f * p1.x() - 3.0f * p2.x() + p3.x()) * t3);

    double retY = 0.5f * ((2.0f * p1.y()) +
    (-p0.y() + p2.y()) * t +
    (2.0f * p0.y() - 5.0f * p1.y() + 4 * p2.y() - p3.y()) * t2 +
    (-p0.y() + 3.0f * p1.y() - 3.0f * p2.y() + p3.y()) * t3);

    return XYCoord(retX,retY);
}

static void populateSplinePoint(PatternShapePointVectorPtr &curvePoints, XYCoord p1, XYCoord p2, XYCoord p3, XYCoord p4, double t)
{
    assert(t>=0.0);
    assert(t<=1.0);
    XYCoord curvePoint = pointOnCurve(p1,p2,p3,p4,t);
    PatternShapePoint shapePt(curvePoint.x(),curvePoint.y());
    curvePoints->push_back(shapePt);
}

static void populateSplinePoints(PatternShapePointVectorPtr &curvePoints, XYCoord p1, XYCoord p2, XYCoord p3, XYCoord p4)
{
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.10);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.20);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.30);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.40);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.50);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.60);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.70);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.80);
    populateSplinePoint(curvePoints, p1,p2,p3,p4,0.90);
}


static PatternShapePointVectorPtr  populateExtrapolatedSplineShape(const XYCoordVector &extendedControlPoints)
{
    assert(extendedControlPoints.size() >= 4);

    PatternShapePointVectorPtr shapePoints(new PatternShapePointVector());

    unsigned int currEndControlIndex = 3;
    for(currEndControlIndex = 3; currEndControlIndex < extendedControlPoints.size(); currEndControlIndex++)
    {

        XYCoord p1 = extendedControlPoints[currEndControlIndex-3];
        XYCoord p2 = extendedControlPoints[currEndControlIndex-2];
        XYCoord p3 = extendedControlPoints[currEndControlIndex-1];
        XYCoord p4 = extendedControlPoints[currEndControlIndex-0];

        assert(p2.x()>p1.x());
        assert(p3.x()>p2.x());
        assert(p4.x()>p3.x());

        // Only populate the first control point in each loop iteration.
        // The last control point is populated after the loop.
        shapePoints->push_back(PatternShapePoint(p2.x(),p2.y()));

        // Populate the intermediate spline points between p2 and p3,
        // using p1 and p4 as control points.
        populateSplinePoints(shapePoints,p1,p2,p3,p4);
    }

    XYCoord lastCoord = extendedControlPoints[extendedControlPoints.size()-2];
    shapePoints->push_back(PatternShapePoint(lastCoord.x(),lastCoord.y()));

    return shapePoints;


}

PatternShapePointVectorPtr PatternShapeCurveGenerator::createCurvedShape(const XYCoordVector &shapePoints)
{
    assert(shapePoints.size() >= 2);

    XYCoordVector shapeControlPoints;
    LinearEquation prefixControlPointEq(shapePoints[0],shapePoints[1]);
    double prefixXVal = shapePoints[0].x()-EXTRAPOLATED_CONTROL_POINT_X_VALUE_OFFSET;
    XYCoord extrapolatedPrefixControlPoint(prefixXVal,prefixControlPointEq.yVal(prefixXVal));
    shapeControlPoints.push_back(extrapolatedPrefixControlPoint);

    shapeControlPoints.insert(shapeControlPoints.end(),shapePoints.begin(),shapePoints.end());

    unsigned int lastIndex = shapePoints.size()-1;
    LinearEquation suffixControlPointEq(shapePoints[lastIndex-1],shapePoints[lastIndex]);
    double suffixXVal = shapePoints[lastIndex].x()+EXTRAPOLATED_CONTROL_POINT_X_VALUE_OFFSET;
    XYCoord extrapolatedSuffixControlPoint(suffixXVal,suffixControlPointEq.yVal(suffixXVal));
    shapeControlPoints.push_back(extrapolatedSuffixControlPoint);

    return populateExtrapolatedSplineShape(shapeControlPoints);

}
