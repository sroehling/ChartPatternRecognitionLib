#ifndef PATTERNSHAPEGENERATOR_H
#define PATTERNSHAPEGENERATOR_H

#include "PatternMatchVisitor.h"
#include "PeriodVal.h"
#include "PatternMatch.h"
#include "PatternShapePoint.h"
#include "PatternShape.h"

class PatternShapeGenerator : public PatternMatchVisitor
{
private:
    PatternShapePointVectorPtr shapePoints_;
    PatternShapePtr patternShape_;
    bool firstSubPatternVisited_;
public:
    PatternShapeGenerator();

    PatternShapePtr generateShape(PatternMatch &patternMatch);

    virtual void visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &);
    virtual void visitVPatternMatch(VPatternMatch &);
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &);
    virtual void visitCupPatternMatch(CupPatternMatch &);

};

#endif // PATTERNSHAPEGENERATOR_H
