#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricWedgePatternMatch.h"
#include "FlatBasePatternMatch.h"
#include "CupPatternMatch.h"
#include "CupWithHandlePatternMatch.h"
#include "DoubleBottomPatternMatch.h"
#include "CupWithoutHandlePatternMatch.h"

PatternShapeGenerator::PatternShapeGenerator()
{
}

PatternShapePtr PatternShapeGenerator::generateShape(PatternMatch &patternMatch)
{
    patternShape_ = PatternShapePtr(new PatternShape());

    breakoutInfo_ = PatternMatchBreakoutInfoPtr(); // reset to NULL (smart pointer)

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

    XYCoord startVCoord = vMatch.firstValue().highCoord();
    vShapePoints->push_back(PatternShapePoint(startVCoord.x(),startVCoord.y()));

    XYCoord midVCoord = vMatch.downTrend()->lastValue().lowCoord();
    vShapePoints->push_back(PatternShapePoint(midVCoord.x(),midVCoord.y()));

    // Plot either the high value or breakout/confirmation level as the last point.
    // See the comment for cup shape generation.
    XYCoord endVCoord =  vMatch.lastValue().highCoord();
    if(breakoutInfo_)
    {
        endVCoord = breakoutInfo_->xyCoord();
    }

    vShapePoints->push_back(PatternShapePoint(endVCoord.x(),endVCoord.y()));

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

static void populateSplinePoints(PatternShapePointVectorPtr &curvePoints,
           XYCoord p1, XYCoord p2, XYCoord p3, XYCoord p4, double maxY)
{
    double splineTInterval = 0.05;
    double currSplineT = splineTInterval;
    XYCoord currPoint = pointOnCurve(p1,p2,p3,p4,currSplineT);

    while((currSplineT < 1.0) && (currPoint.y() < maxY))
    {
        PatternShapePoint shapePt(currPoint.x(),currPoint.y());
        curvePoints->push_back(shapePt);

        currSplineT += splineTInterval;
        currPoint = pointOnCurve(p1,p2,p3,p4,currSplineT);
    }
}



void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    PatternShapePointVectorPtr cupPoints(new PatternShapePointVector());


    PeriodVal startCupVal = cupMatch.downTrend()->firstValue();

    // Although the pattern matching algorithms use typical values,
    // a cup technically starts with the high of the first value.
    // TODO - Consider updating/evaluating the pattern matching to use
    // the high value as the first value in the initial downtrend line.
    // However, to the extent a high value doesn't reflect the true conviction of
    // the traders (perhaps just a whipsaw for intraday trading), arguably
    // typical values (or even closing values) are still better for the
    // underlying pattern match.
    XYCoord startCupCoord = startCupVal.highCoord();
    XYCoord startCupBottomCoord = cupMatch.cupBottom()->firstValue().typicalCoord();
    XYCoord endCupBottomCoord = cupMatch.cupBottom()->lastValue().typicalCoord();
    PeriodVal endCupVal = cupMatch.upTrend()->lastValue();

    // The high coordinate is applicable for the LHS of the cup with handle.
    XYCoord endCupCoord =  endCupVal.highCoord();
    if(breakoutInfo_)
    {
        endCupCoord = breakoutInfo_->xyCoord();
    }

    cupPoints->push_back(PatternShapePoint(startCupCoord.x(),startCupCoord.y()));

    XYCoord extendedDownTrendCoord(extrapolatedMatchCoord(
                                cupMatch.downTrend(),cupMatch.downTrend()->firstValue(),-2.0));
    populateSplinePoints(cupPoints,extendedDownTrendCoord,
                         startCupCoord,startCupBottomCoord,endCupBottomCoord);

    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->firstValue()));

    populateSplinePoints(cupPoints,startCupCoord,
                         startCupBottomCoord,endCupBottomCoord,endCupCoord);

    cupPoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->lastValue()));

    // If this cup is being generated with a breakout price in place, then don't extend
    // the line beyond the breakout price.
    double maxY = endCupCoord.y();
    if(breakoutInfo_)
    {
        maxY = breakoutInfo_->breakoutPrice();
    }

    XYCoord extendedUpTrendCoord(extrapolatedMatchCoord(cupMatch.upTrend(),endCupVal,2.0));
    populateSplinePoints(cupPoints,startCupBottomCoord,
                         endCupBottomCoord,endCupCoord,extendedUpTrendCoord,maxY);

    cupPoints->push_back(PatternShapePoint(endCupCoord.x(),endCupCoord.y()));

    patternShape_->addLineShape(cupPoints);

}


void PatternShapeGenerator::visitDoubleBottomMiddle(DoubleBottomPatternMatch &doubleBottom)
{
    breakoutInfo_ = doubleBottom.breakoutInfo;
}

void PatternShapeGenerator::visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &)
{
    // No-op: pattern shape generation handled by visitVPatternMatch
}

void PatternShapeGenerator::visitCupWithoutHandleStart(CupWithoutHandlePatternMatch &cupWithoutHandle)
{
    // Initialize the breakoutInfo_, so the next cup pattern generation will use this as the end-point
    // for drawing the pattern.
    breakoutInfo_ = cupWithoutHandle.breakoutInfo;
}


void PatternShapeGenerator::visitCupWithoutHandlePatternMatch(CupWithoutHandlePatternMatch &)
{
    // No-op: pattern shape generation handled in visitCupPatternMatch
}

void PatternShapeGenerator::visitCupWithHandleHandleStart(CupWithHandlePatternMatch &cupWithHandle)
{
    // Initialize the breakoutInfo_, so the next cup pattern generation will use this as the end-point
    // for drawing the pattern.
    breakoutInfo_ = cupWithHandle.breakoutInfo;
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

    XYCoord interceptCoord = wedge.trendLineIntercept();
    PatternShapePoint interceptPt(interceptCoord.x(),interceptCoord.y());
    upperShapePoints->push_back(interceptPt);
    lowerShapePoints->push_back(interceptPt);

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
