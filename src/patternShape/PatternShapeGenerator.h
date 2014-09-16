#ifndef PATTERNSHAPEGENERATOR_H
#define PATTERNSHAPEGENERATOR_H

#include "PatternMatchVisitor.h"
#include "PeriodVal.h"
#include "PatternMatch.h"
#include "PatternShapePoint.h"
#include "PatternShape.h"
#include "PatternMatchBreakoutInfo.h"

class PatternShapeGenerator : public PatternMatchVisitor
{
private:
    PatternShapePtr patternShape_;

    PatternMatchBreakoutInfoPtr breakoutInfo_;

    void generateWedgeShape(const PatternMatch &match,
          const LinearEquationPtr &upperTrendlineEq, const LinearEquationPtr &lowerTrendLineEq);

public:
    PatternShapeGenerator();

    PatternShapePtr generateShape(PatternMatch &patternMatch);

    virtual void visitTrianglePatternMatch(TrianglePatternMatch&);

    virtual void visitSymetricWedgePatternMatch(SymetricTrianglePatternMatch &);

    virtual void visitDescendingTrianglePatternMatch(DescendingTrianglePatternMatch &);
    virtual void visitAscendingTrianglePatternMatch(AscendingTrianglePatternMatch &);

    virtual void visitRectanglePatternMatch(RectanglePatternMatch &);

    virtual void visitWedgePatternMatch(WedgePatternMatch &);

    virtual void visitVPatternMatch(VPatternMatch &);

    virtual void visitDoubleBottomMiddle(DoubleBottomPatternMatch &);
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &);

    virtual void visitCupPatternMatch(CupPatternMatch &);

    virtual void visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &);
    virtual void visitCupWithHandleHandleStart(CupWithHandlePatternMatch &);

    virtual void visitCupWithoutHandleStart(CupWithoutHandlePatternMatch &);
    virtual void visitCupWithoutHandlePatternMatch(CupWithoutHandlePatternMatch &);

};

#endif // PATTERNSHAPEGENERATOR_H
