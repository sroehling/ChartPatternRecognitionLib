#ifndef PERVALCLTNSLIDINGWINDOW_H
#define PERVALCLTNSLIDINGWINDOW_H

#include "PeriodVal.h"

class PerValCltnSlidingWindow
{
private:
    PeriodValCltn::iterator cltnBegin_;
    PeriodValCltn::iterator cltnEnd_;

    PeriodValCltn::iterator windowBegin_;
    PeriodValCltn::iterator windowMiddle_;
    PeriodValCltn::iterator windowEnd_;

    unsigned int windowSize_;
public:
    PerValCltnSlidingWindow(unsigned int windowSize,
             const PeriodValCltn::iterator &cltnBegin,
             const PeriodValCltn::iterator &cltnEnd);

    void resetToCltnBegin();
    bool windowAtEnd() const;
    void advanceWindow();

    const PeriodValCltn::iterator &windowBegin() const;
    PeriodValCltn::iterator windowEnd() const;


    const PeriodValCltn::iterator &windowFirst() const;
    const PeriodValCltn::iterator &windowMiddle() const;
    const PeriodValCltn::iterator &windowLast() const;

    const PeriodVal &firstVal() const;
    const PeriodVal &lastVal() const;
    const PeriodVal &middleVal() const;

    static bool windowFitsWithinRange(unsigned int windowSize,
           const PeriodValCltn::iterator &cltnBegin,
           const PeriodValCltn::iterator &cltnEnd);

};

#endif // PERVALCLTNSLIDINGWINDOW_H
