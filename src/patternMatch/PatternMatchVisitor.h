/*
 * PatternMatchVisitor.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVISITOR_H_
#define PATTERNMATCHVISITOR_H_

class WedgePatternMatch;

class PatternMatchVisitor {
public:
	PatternMatchVisitor();

	virtual void visitWedgePatternMatch(WedgePatternMatch &wedgePatternMatch) {}

	virtual ~PatternMatchVisitor();
};

#endif /* PATTERNMATCHVISITOR_H_ */
