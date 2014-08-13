#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"
#include "SymetricWedgePatternMatch.h"

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
        shapePoints_->push_back(PatternShapePoint(vMatch.startTime(),vMatch.firstValue().high()));
    }
    firstSubPatternVisited_ = true;

    shapePoints_->push_back(PatternShapePoint(vMatch.lowestLowTime(),vMatch.lowestLow()));
    shapePoints_->push_back(PatternShapePoint(vMatch.endTime(),vMatch.lastValue().high()));

}

void PatternShapeGenerator::visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &)
{
    firstSubPatternVisited_ = true;
    patternShape_->addCurveShape(shapePoints_);

}


void PatternShapeGenerator::visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &symetricWedge)
{

    firstSubPatternVisited_ = true;

    // The starting point for drawing the upper and lower trendlines is the first value in upper trend line.
    LinearEquationPtr upperTrendLineEq = symetricWedge.upperTrendLine()->segmentEq();
    LinearEquationPtr lowerTrendLineEq = symetricWedge.lowerTrendLine()->segmentEq();

    PeriodValCltn::iterator beginTrendlineShape = symetricWedge.patternBeginIter();
    PeriodValCltn::iterator endTrendlineShape = symetricWedge.interceptEndIter();

    PatternShapePointVectorPtr upperShapePoints(new PatternShapePointVector());
    PatternShapePointVectorPtr lowerShapePoints(new PatternShapePointVector());

    for(PeriodValCltn::iterator drawTrendLineIter = beginTrendlineShape;
        drawTrendLineIter != endTrendlineShape;
        drawTrendLineIter++)
    {
        double xVal = (*drawTrendLineIter).pseudoXVal();

        double upperYVal = upperTrendLineEq->yVal(xVal);
        upperShapePoints->push_back(PatternShapePoint((*drawTrendLineIter).periodTime(),upperYVal));

        double lowerYVal = lowerTrendLineEq->yVal(xVal);
        lowerShapePoints->push_back(PatternShapePoint((*drawTrendLineIter).periodTime(),lowerYVal));
    }
    patternShape_->addCurveShape(upperShapePoints);
    patternShape_->addCurveShape(lowerShapePoints);

}
