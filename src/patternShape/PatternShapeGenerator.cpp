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

    shapePoints_ = PatternShapePointVectorPtr(new PatternShapePointVector());
    firstSubPatternVisited_ = false;

    patternMatch.acceptVisitor(*this);

    assert(patternShape_->numCurveShapes() > 0);

    return patternShape_;
}

static PatternShapePoint createTypicalPriceShapePoint(const PeriodVal &perVal)
{
    return PatternShapePoint(perVal.pseudoXVal(),
                             perVal.periodTime(),perVal.typicalPrice());
}

void PatternShapeGenerator::visitVPatternMatch(VPatternMatch &vMatch)
{
    if(!firstSubPatternVisited_)
    {
        PeriodVal vStartPoint = vMatch.firstValue();
        shapePoints_->push_back(PatternShapePoint(createTypicalPriceShapePoint(vStartPoint)));
    }
    firstSubPatternVisited_ = true;

    PeriodVal vMidPoint = vMatch.downTrend()->lastValue();
    shapePoints_->push_back(createTypicalPriceShapePoint(vMidPoint));

    PeriodVal vEndPoint = vMatch.lastValue();
    shapePoints_->push_back(createTypicalPriceShapePoint(vEndPoint));

}

void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    PatternShapePointVectorPtr cupShapePoints(new PatternShapePointVector());

    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.downTrend()->firstValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->firstValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.cupBottom()->lastValue()));
    cupShapePoints->push_back(createTypicalPriceShapePoint(cupMatch.upTrend()->lastValue()));

    patternShape_->addCurveShape(cupShapePoints);

}

void PatternShapeGenerator::visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &)
{
    firstSubPatternVisited_ = true;
    patternShape_->addCurveShape(shapePoints_);

}

void PatternShapeGenerator::visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &)
{
    // The cup with handle will consist of a cup pattern, followed by either a V pattern or smaller cup.
    // A cup shaped handle will add it's own curve shape, while a V shape will populate entries in
    // patternShape_.
    if(shapePoints_->size() > 0)
    {
        patternShape_->addCurveShape(shapePoints_);
    }
}

void PatternShapeGenerator::visitWedgePatternMatch(WedgePatternMatch &wedge)
{
    firstSubPatternVisited_ = true;

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
        upperShapePoints->push_back(PatternShapePoint(xVal,
                           (*drawTrendLineIter).periodTime(),upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,
                                       (*drawTrendLineIter).periodTime(),lowerYVal));
    }
    patternShape_->addCurveShape(upperShapePoints);
    patternShape_->addCurveShape(lowerShapePoints);

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
        upperShapePoints->push_back(PatternShapePoint(xVal,
                           (*drawTrendLineIter).periodTime(),upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint(xVal,
                                       (*drawTrendLineIter).periodTime(),lowerYVal));
    }
    patternShape_->addCurveShape(upperShapePoints);
    patternShape_->addCurveShape(lowerShapePoints);

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
