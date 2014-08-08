#ifndef PATTERNSHAPEGENERATOR_H
#define PATTERNSHAPEGENERATOR_H

#include "PatternMatchVisitor.h"
#include "PeriodVal.h"
#include "PatternMatch.h"
#include "PatternShapePoint.h"

class PatternShapeGenerator : public PatternMatchVisitor
{
private:
    PatternShapePointVectorPtr shapePoints_;
    bool firstSubPatternVisited_;
public:
    PatternShapeGenerator();

    PatternShapePointVectorPtr generateShape(PatternMatch &patternMatch);

    virtual void visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &) {}
    virtual void visitVPatternMatch(VPatternMatch &);
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &);

};

#endif // PATTERNSHAPEGENERATOR_H
