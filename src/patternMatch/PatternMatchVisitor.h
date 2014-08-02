/*
 * PatternMatchVisitor.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVISITOR_H_
#define PATTERNMATCHVISITOR_H_

class SymetricWedgePatternMatch;

class PatternMatchVisitor {
public:
	PatternMatchVisitor();

	virtual void visitSymetricWedgePatternMatch(SymetricWedgePatternMatch &wedgePatternMatch) {}

	virtual ~PatternMatchVisitor();
};

#endif /* PATTERNMATCHVISITOR_H_ */
