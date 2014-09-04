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
    PatternShapePtr patternShape_;
public:
    PatternShapeGenerator();

    PatternShapePtr generateShape(PatternMatch &patternMatch);

    virtual void visitWedgePatternMatch(WedgePatternMatch&);
    virtual void visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &);
    virtual void visitFallingWedgePatternMatch(FallingWedgePatternMatch &);
    virtual void visitRisingWedgePatternMatch(RisingWedgePatternMatch &);
    virtual void visitFlatBasePatternMatch(FlatBasePatternMatch &);

    virtual void visitVPatternMatch(VPatternMatch &);
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &);
    virtual void visitCupPatternMatch(CupPatternMatch &);
    virtual void visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &);
    virtual void visitCupWithoutHandlePatternMatch(CupWithoutHandlePatternMatch &);

};

#endif // PATTERNSHAPEGENERATOR_H
