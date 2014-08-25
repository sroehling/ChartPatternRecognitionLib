#ifndef PATTERNSHAPEPOINT_H
#define PATTERNSHAPEPOINT_H

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>


class PatternShapePoint
{
private:
    double pseudoXVal_;
    boost::posix_time::ptime periodTime_;
    double periodVal_;
public:
    PatternShapePoint(double pseudoXVal,const boost::posix_time::ptime &periodTime, double periodVal);
    boost::posix_time::ptime periodTime() const { return periodTime_; }
    double periodVal() const { return periodVal_; }
    double pseudoXVal() const { return pseudoXVal_; }
};

typedef std::vector<PatternShapePoint> PatternShapePointVector;
typedef boost::shared_ptr<PatternShapePointVector> PatternShapePointVectorPtr;

typedef std::vector<PatternShapePointVectorPtr> PatternShapePointVectorVector;
typedef boost::shared_ptr<PatternShapePointVectorVector> PatternShapePointVectorVectorPtr;

#endif // PATTERNSHAPEPOINT_H
