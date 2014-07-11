/*
 * RecoverPercentOfDepth.h
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#ifndef RECOVERPERCENTOFDEPTH_H_
#define RECOVERPERCENTOFDEPTH_H_

#include <PatternMatchValidatorFactory.h>

class RecoverPercentOfDepth: public PatternMatchValidatorFactory {
private:
	double depthPercRecover_;
public:
	RecoverPercentOfDepth(double depthPercRecover);

	virtual PatternMatchValidatorPtr createValidator(const PatternMatchPtr &otherMatch) const;

	virtual ~RecoverPercentOfDepth();
};

#endif /* RECOVERPERCENTOFDEPTH_H_ */
