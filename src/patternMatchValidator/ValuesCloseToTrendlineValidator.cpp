#include "ValuesCloseToTrendlineValidator.h"
#include "ChartSegment.h"
#include "DoubleRange.h"
#include "MathHelper.h"
#include <assert.h>

ValuesCloseToTrendlineValidator::ValuesCloseToTrendlineValidator()
{
}

bool valuesBoundedWithinThresholdLines(const PeriodValSegmentPtr &trendlineSeg,
         const LinearEquation &upperThresholdEq, const LinearEquation &lowerThresholdEq)
{
    for(PeriodValCltn::iterator valIter = trendlineSeg->segBegin();
        valIter != trendlineSeg->segEnd(); valIter++)
    {
        // Use the closing values to test whether the value is within the threshold lines.
        // Alternately, the high and low values could represent "shake-out" values which
        // don't accurately reflect conviction of the traders like the close does.
        if(!upperThresholdEq.belowLine((*valIter).closeCoord()))
        {
            return false;
        }
        else if(!lowerThresholdEq.aboveLine((*valIter).closeCoord()))
        {
            return false;
        }
    }
    return true;
}

bool ValuesCloseToTrendlineValidator::validPattern(const PatternMatch &candidateMatch)
{
    assert(candidateMatch.numPeriods() > 1);

    PeriodValSegmentPtr matchSeg = candidateMatch.matchSegment();

    // The approach here is to draw an upper and lower channel line parallel to the original trend line. Conceptually, a box
    // is drawn around the trend-line end-points and the parallel to the original trend line. Since the corners of the box
    // are right angles, we can use the pythagorean theorem to ensure the resulting channel lines run parallel.
    // Basically, since both the x and y offsets from the corner of the box of the new channel lines are proportional to the the original trend
    // line, but drawn at a different location, the angles (determined from the ratios of these offsets (a and b in pyth. theor.)) are
    // the same.
    //
    // TBD - If the trend lines are step, resulting in a short or tall rectangular box, will this result in channel lines which give enough room
    // for the values to oscillate without unnecessarily disqualifying the candidateMatch?
    //
    // See the ValuesCloseToTrendLineValidatorMathComment.pdf for a diagram explaining the math to calculate the channel lines.

    ChartSegmentPtr trendLineChartSeg(new ChartSegment(candidateMatch.matchSegment()));

    double firstY = matchSeg->firstVal().typicalPrice();
    double lastY = matchSeg->lastVal().typicalPrice();

    double firstX = matchSeg->firstVal().pseudoXVal();
    double lastX = matchSeg->lastVal().pseudoXVal();

    if(trendLineChartSeg->slope() < 0.0)
    {
        double upperYMidPerc = 0.4;
        double upperXMidPerc = 0.4;
        double upperXMid = DoubleRange(firstX,lastX).percentMidpointVal(upperXMidPerc);
        double upperYMid = DoubleRange(lastY,firstY).percentMidpointVal(upperYMidPerc);
        LinearEquation upperThresholdEq(XYCoord(upperXMid,firstY),XYCoord(lastX, upperYMid));

        double lowerYMidPerc = 0.6;
        double lowerXMidPerc = 0.6;
        double lowerXMid = DoubleRange(firstX,lastX).percentMidpointVal(lowerXMidPerc);
        double lowerYMid = DoubleRange(lastY,firstY).percentMidpointVal(lowerYMidPerc);
        LinearEquation lowerThresholdEq(XYCoord(firstX, lowerYMid),XYCoord(lowerXMid, lastY));

        // The upper and lower threshold/channel lines are drawn parallel to the original trend-line.
        assert(MathHelper::valuesClose(trendLineChartSeg->slope(),upperThresholdEq.slope(),0.001));
        assert(MathHelper::valuesClose(trendLineChartSeg->slope(),lowerThresholdEq.slope(),0.001));


        if(valuesBoundedWithinThresholdLines(matchSeg,upperThresholdEq,lowerThresholdEq))
        {
            return true;
        }
     }
    else // slope > 0.0; i.e. uptrend line
    {
        double upperXMidPerc = 0.6;
        double upperYMidPerc = 0.4;
        double upperXMid = DoubleRange(firstX,lastX).percentMidpointVal(upperXMidPerc);
        double upperYMid = DoubleRange(firstY,lastY).percentMidpointVal(upperYMidPerc);
        LinearEquation upperThresholdEq(XYCoord(firstX, upperYMid),XYCoord(upperXMid, lastY));

        double lowerXMidPerc = 0.4;
        double lowerYMidPerc = 0.6;
        double lowerXMid = DoubleRange(firstX,lastX).percentMidpointVal(lowerXMidPerc);
        double lowerYMid = DoubleRange(firstY,lastY).percentMidpointVal(lowerYMidPerc);
        LinearEquation lowerThresholdEq(XYCoord(lowerXMid, firstY),XYCoord(lastX, lowerYMid));

        // The upper and lower threshold/channel lines are drawn parallel to the original trend-line.
        assert(MathHelper::valuesClose(trendLineChartSeg->slope(),upperThresholdEq.slope(),0.001));
        assert(MathHelper::valuesClose(trendLineChartSeg->slope(),lowerThresholdEq.slope(),0.001));

        if(valuesBoundedWithinThresholdLines(matchSeg,upperThresholdEq,lowerThresholdEq))
        {
            return true;
        }

    }

    return false;
}
