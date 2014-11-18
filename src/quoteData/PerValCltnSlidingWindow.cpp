#include "PerValCltnSlidingWindow.h"


static void advanceStopAtEnd(PeriodValCltn::iterator &iter, const PeriodValCltn::iterator &endIter, unsigned int advanceCount)
{
    for(unsigned int advanceCnt = 0; advanceCnt < advanceCount; advanceCnt++)
    {
        iter++;
        if(iter == endIter)
        {
            return;
        }
    }
}

void PerValCltnSlidingWindow::resetToCltnBegin()
{
    windowBegin_ = cltnBegin_;

    windowEnd_ = cltnBegin_;
    advanceStopAtEnd(windowEnd_,cltnEnd_,windowSize_);
    assert(windowEnd_ != cltnEnd_);

    windowMiddle_ = cltnBegin_;
    advanceStopAtEnd(windowMiddle_,cltnEnd_,windowSize_/2);
    assert(windowMiddle_ != cltnEnd_);

}

void PerValCltnSlidingWindow::advanceWindow()
{
    assert(!windowAtEnd());
    windowEnd_++;
    windowBegin_++;
    windowMiddle_++;
}

bool PerValCltnSlidingWindow::windowAtEnd() const
{
    return (windowEnd_ == cltnEnd_)?true:false;
}

const PeriodValCltn::iterator &PerValCltnSlidingWindow::windowFirst() const
{
    assert(!windowAtEnd());
    return windowBegin_;
}

const PeriodValCltn::iterator &PerValCltnSlidingWindow::windowBegin() const
{
    assert(!windowAtEnd());
    return windowFirst();
}


const PeriodValCltn::iterator &PerValCltnSlidingWindow::windowMiddle() const
{
    assert(!windowAtEnd());
     return windowMiddle_;

}

const PeriodValCltn::iterator &PerValCltnSlidingWindow::windowLast() const
{
    assert(!windowAtEnd());
    return windowEnd_;
}

PeriodValCltn::iterator PerValCltnSlidingWindow::windowEnd() const
{
    assert(!windowAtEnd());

    PeriodValCltn::iterator winEnd = windowEnd_;
    winEnd++;
    return winEnd;
}


const PeriodVal &PerValCltnSlidingWindow::firstVal() const
{
    return (*windowFirst());
}

const PeriodVal &PerValCltnSlidingWindow::middleVal() const
{
    return (*windowMiddle());

}

const PeriodVal &PerValCltnSlidingWindow::lastVal() const
{
    return (*windowLast());
}


PerValCltnSlidingWindow::PerValCltnSlidingWindow(unsigned int windowSize,
         const PeriodValCltn::iterator &cltnBegin,
         const PeriodValCltn::iterator &cltnEnd)
    : cltnBegin_(cltnBegin), cltnEnd_(cltnEnd), windowSize_(windowSize)
{
    assert(windowSize > 0);
    assert(cltnBegin_ != cltnEnd_);
    assert(PerValCltnSlidingWindow::windowFitsWithinRange(windowSize,cltnBegin,cltnEnd));

    resetToCltnBegin();

}


bool PerValCltnSlidingWindow::windowFitsWithinRange(unsigned int windowSize,
       const PeriodValCltn::iterator &cltnBegin,
       const PeriodValCltn::iterator &cltnEnd)
{
    assert(windowSize > 0);
    return (std::distance(cltnBegin,cltnEnd) >= (int)windowSize)?true:false;
}
