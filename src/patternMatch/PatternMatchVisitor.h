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
class TrianglePatternMatch;
class AscendingTrianglePatternMatch;
class CupWithHandlePatternMatch;
class CupWithoutHandlePatternMatch;
class RectanglePatternMatch;
class WedgePatternMatch;

class PatternMatchVisitor {
public:
	PatternMatchVisitor();

    virtual void visitTrianglePatternMatch(TrianglePatternMatch&) {}

    // Triangles
    virtual void visitSymetricWedgePatternMatch(SymetricTrianglePatternMatch &) {}
    virtual void visitDescendingTrianglePatternMatch(DescendingTrianglePatternMatch &) {}
    virtual void visitAscendingTrianglePatternMatch(AscendingTrianglePatternMatch &) {}

    virtual void visitWedgePatternMatch(WedgePatternMatch &) {}

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
