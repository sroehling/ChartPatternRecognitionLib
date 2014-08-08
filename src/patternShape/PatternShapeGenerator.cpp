#include "PatternShapeGenerator.h"
#include "VPatternMatch.h"

PatternShapeGenerator::PatternShapeGenerator()
{
}

PatternShapePointVectorPtr PatternShapeGenerator::generateShape(PatternMatch &patternMatch)
{
    shapePoints_ = PatternShapePointVectorPtr(new PatternShapePointVector());
    firstSubPatternVisited_ = false;

    patternMatch.acceptVisitor(*this);

    return shapePoints_;
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

}
