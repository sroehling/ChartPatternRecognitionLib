/*
 * PatternMatchVisitor.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVISITOR_H_
#define PATTERNMATCHVISITOR_H_

class SymetricTrianglePatternMatch;
class DescendingTrianglePatternMatch;
class VPatternMatch;
class DoubleBottomPatternMatch;
class CupPatternMatch;
class WedgePatternMatch;
class AscendingTrianglePatternMatch;
class CupWithHandlePatternMatch;
class CupWithoutHandlePatternMatch;
class RectanglePatternMatch;

class PatternMatchVisitor {
public:
	PatternMatchVisitor();

    virtual void visitWedgePatternMatch(WedgePatternMatch&) {}

    // Triangles
    virtual void visitSymetricWedgePatternMatch(SymetricTrianglePatternMatch &) {}
    virtual void visitFallingWedgePatternMatch(DescendingTrianglePatternMatch &) {}
    virtual void visitRisingWedgePatternMatch(AscendingTrianglePatternMatch &) {}

    virtual void visitRectanglePatternMatch(RectanglePatternMatch &) {}

    virtual void visitVPatternMatch(VPatternMatch &) {}

    virtual void visitDoubleBottomMiddle(DoubleBottomPatternMatch &) {}
    virtual void visitDoubleBottomPatternMatch(DoubleBottomPatternMatch &) {}

    virtual void visitCupPatternMatch(CupPatternMatch &) {}

    virtual void visitCupWithHandlePatternMatch(CupWithHandlePatternMatch &) {}
    virtual void visitCupWithHandleHandleStart(CupWithHandlePatternMatch &) {}

    virtual void visitCupWithoutHandleStart(CupWithoutHandlePatternMatch &) {}
    virtual void visitCupWithoutHandlePatternMatch(CupWithoutHandlePatternMatch &) {}

	virtual ~PatternMatchVisitor();
};

#endif /* PATTERNMATCHVISITOR_H_ */
