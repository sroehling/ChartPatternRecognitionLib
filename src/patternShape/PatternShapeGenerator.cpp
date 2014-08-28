#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricWedgePatternMatch.h"
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


void PatternShapeGenerator::visitVPatternMatch(VPatternMatch &vMatch)
{
    if(!firstSubPatternVisited_)
    {
        shapePoints_->push_back(PatternShapePoint(vMatch.firstValue().pseudoXVal(),
                                                  vMatch.startTime(),vMatch.firstValue().high()));
    }
    firstSubPatternVisited_ = true;

    shapePoints_->push_back(PatternShapePoint(
                                vMatch.lowestLowVal().pseudoXVal(),
                                vMatch.lowestLowTime(),vMatch.lowestLow()));
    shapePoints_->push_back(PatternShapePoint(vMatch.lastValue().pseudoXVal(),
                                vMatch.endTime(),vMatch.lastValue().high()));

}

void PatternShapeGenerator::visitCupPatternMatch(CupPatternMatch &cupMatch)
{
    PatternShapePointVectorPtr cupShapePoints(new PatternShapePointVector());

    cupShapePoints->push_back(PatternShapePoint(
                                  cupMatch.downTrend()->firstValue().pseudoXVal(),
                                  cupMatch.downTrend()->firstValue().periodTime(),
                              cupMatch.downTrend()->firstValue().high()));
    cupShapePoints->push_back(PatternShapePoint(
                                  cupMatch.cupBottom()->firstValue().pseudoXVal(),
                                  cupMatch.cupBottom()->firstValue().periodTime(),
                              cupMatch.cupBottom()->firstValue().typicalPrice()));
    cupShapePoints->push_back(PatternShapePoint(
                                  cupMatch.cupBottom()->lastValue().pseudoXVal(),
                                  cupMatch.cupBottom()->lastValue().periodTime(),
                              cupMatch.cupBottom()->lastValue().typicalPrice()));
    cupShapePoints->push_back(PatternShapePoint(
                                   cupMatch.upTrend()->lastValue().pseudoXVal(),
                                  cupMatch.upTrend()->lastValue().periodTime(),
                              cupMatch.upTrend()->lastValue().close()));

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
