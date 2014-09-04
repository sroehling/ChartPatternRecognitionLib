#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricWedgePatternMatch.h"
#include "FlatBasePatternMatch.h"
#include "CupPatternMatch.h"

PatternShapeGenerator::PatternShapeGenerator()
{
}

PatternShapePtr PatternShapeGenerator::generateShape(PatternMatch &patternMatch)
{
    patternShape_ = PatternShapePtr(new PatternShape());

    patternMatch.acceptVisitor(*this);

    assert(patternShape_->numShapes() > 0);

    return patternShape_;
}

static PatternShapePoint createTypicalPriceShapePoint(const PeriodVal &perVal)
{
    return PatternShapePoint(perVal.pseudoXVal(),perVal.typicalPrice());
}


void PatternShapeGenerator::visitVPatternMatch(VPatternMatch &vMatch)
{
    PatternShapePointVectorPtr vShapePoints(new PatternShapePointVector());

    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.firstValue()));
    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.downTrend()->lastValue()));
    vShapePoints->push_back(createTypicalPriceShapePoint(vMatch.lastValue()));

    patternShape_->addLineShape(vShapePoints);
}

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

static XYCoord extrapolatedMatchCoord(const PatternMatchPtr &match, const PeriodVal &startingVal, double xOffset)
{
    assert(match->segments().size() == 1);
    ChartSegmentPtr matchChartSegment = match->segments().front();

    PeriodValCltn::iterator midPointIter = match->beginMatchIter();
    std::advance(midPointIter,match->numPeriods()/2);

    double startingXVal = startingVal.pseudoXVal();
    double offsetXVal = startingXVal + xOffset;
    XYCoord offsetCoord(
                offsetXVal,
                matchChartSegment->segmentEq()->yVal(offsetXVal));

    return offsetCoord;
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



void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    PatternShapePointVectorPtr cupPoints(new PatternShapePointVector());
    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.downTrend()->firstValue()));

    XYCoord extendedDownTrendCoord(extrapolatedMatchCoord(
                                cupMatch.downTrend(),cupMatch.downTrend()->firstValue(),-2.0));

    populateSplinePoints(cupPoints,extendedDownTrendCoord,
                         cupMatch.downTrend()->firstValue().typicalCoord(),
                         cupMatch.cupBottom()->firstValue().typicalCoord(),
                         cupMatch.cupBottom()->lastValue().typicalCoord());

    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->firstValue()));

    populateSplinePoints(cupPoints,cupMatch.downTrend()->firstValue().typicalCoord(),
                         cupMatch.cupBottom()->firstValue().typicalCoord(),
                         cupMatch.cupBottom()->lastValue().typicalCoord(),
                         cupMatch.upTrend()->lastValue().typicalCoord());

    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->lastValue()));

    XYCoord extendedUpTrendCoord(extrapolatedMatchCoord(
                   cupMatch.upTrend(),cupMatch.upTrend()->lastValue(),2.0));
    populateSplinePoints(cupPoints,cupMatch.cupBottom()->firstValue().typicalCoord(),
                         cupMatch.cupBottom()->lastValue().typicalCoord(),
                         cupMatch.upTrend()->lastValue().typicalCoord(),
                          extendedUpTrendCoord);

    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.upTrend()->lastValue()));

    patternShape_->addLineShape(cupPoints);

}

void PatternShapeGenerator::visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &)
{
    // No-op: pattern shape generation handled by visitVPatternMatch
}

void PatternShapeGenerator::visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &)
{
    // No-op: pattern shape generation handled by visitVPatternMatch and visitCupPatternMatch
    // for both the initial base and handle.
}

void PatternShapeGenerator::visitWedgePatternMatch(WedgePatternMatch &wedge)
{
    // The starting point for drawing the upper and lower trendlines is the first value in upper trend line.
    LinearEquationPtr upperTrendLineEq = wedge.upperTrendLine()->segmentEq();
    LinearEquationPtr lowerTrendLineEq = wedge.lowerTrendLine()->segmentEq();

    PeriodValCltn::iterator beginTrendlineShape = wedge.patternBeginIter();
    PeriodValCltn::iterator endTrendlineShape = wedge.interceptEndIter();

    PatternShapePointVectorPtr upperShapePoints(new PatternShapePointVector());
    PatternShapePointVectorPtr lowerShapePoints(new PatternShapePointVector());

    for(PeriodValCltn::iterator drawTrendLineIter = beginTrendlineShape;
        drawTrendLineIter != endTrendlineShape;
        drawTrendLineIter++)
    {
        double xVal = (*drawTrendLineIter).pseudoXVal();

        double upperYVal = upperTrendLineEq->yVal(xVal);
        upperShapePoints->push_back(PatternShapePoint(xVal,upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,lowerYVal));
    }
    patternShape_->addLineShape(upperShapePoints);
    patternShape_->addLineShape(lowerShapePoints);

}

void PatternShapeGenerator::visitFlatBasePatternMatch(FlatBasePatternMatch &flatBaseMatch)
{
    // The starting point for drawing the upper and lower trendlines is the first value in upper trend line.
    LinearEquationPtr upperTrendLineEq = flatBaseMatch.upperTrendLine()->segmentEq();
    LinearEquationPtr lowerTrendLineEq = flatBaseMatch.lowerTrendLine()->segmentEq();

    PeriodValCltn::iterator beginTrendlineShape = flatBaseMatch.beginMatchIter();
    PeriodValCltn::iterator endTrendlineShape = flatBaseMatch.endMatchIter();

    PatternShapePointVectorPtr upperShapePoints(new PatternShapePointVector());
    PatternShapePointVectorPtr lowerShapePoints(new PatternShapePointVector());

    for(PeriodValCltn::iterator drawTrendLineIter = beginTrendlineShape;
        drawTrendLineIter != endTrendlineShape;
        drawTrendLineIter++)
    {
        double xVal = (*drawTrendLineIter).pseudoXVal();

        double upperYVal = upperTrendLineEq->yVal(xVal);
        upperShapePoints->push_back(PatternShapePoint(xVal,upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,lowerYVal));
    }
    patternShape_->addLineShape(upperShapePoints);
    patternShape_->addLineShape(lowerShapePoints);

}

void PatternShapeGenerator::visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitFallingWedgePatternMatch(FallingWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitRisingWedgePatternMatch(RisingWedgePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}
