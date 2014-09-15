#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricTrianglePatternMatch.h"
#include "FlatBasePatternMatch.h"
#include "CupPatternMatch.h"
#include "CupWithHandlePatternMatch.h"
#include "DoubleBottomPatternMatch.h"
#include "CupWithoutHandlePatternMatch.h"
#include "PatternShapeCurveGenerator.h"

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


void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    // TODO - Consider updating/evaluating the pattern matching to use
    // the high value as the first value in the initial downtrend line.
    // However, to the extent a high value doesn't reflect the true conviction of
    // the traders (perhaps just a whipsaw for intraday trading), arguably
    // typical values (or even closing values) are still better for the
    // underlying pattern match.


    XYCoordVector shapePoints;

    shapePoints.push_back(cupMatch.downTrend()->firstValue().highCoord());
    shapePoints.push_back(cupMatch.cupBottom()->firstValue().typicalCoord());
    shapePoints.push_back(cupMatch.cupBottom()->lastValue().typicalCoord());

    // The high coordinate is applicable for the LHS of the cup with handle.
    // If this cup is being generated with a breakout price in place, then don't extend
    // the line beyond the breakout price.
    XYCoord endCupCoord =  cupMatch.upTrend()->lastValue().highCoord();
    if(breakoutInfo_)
    {
        endCupCoord = breakoutInfo_->xyCoord();
    }

    shapePoints.push_back(endCupCoord);

    patternShape_->addLineShape(PatternShapeCurveGenerator().createCurvedShape(shapePoints));

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

void PatternShapeGenerator::visitSymetricWedgePatternMatch(SymetricTrianglePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitFallingWedgePatternMatch(DescendingTrianglePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}

void PatternShapeGenerator::visitRisingWedgePatternMatch(AscendingTrianglePatternMatch &)
{
    // No-op: pattern shape generation handled by visitWedgePatternMatch
}
