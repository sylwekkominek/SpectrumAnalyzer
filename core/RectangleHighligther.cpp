/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "RectangleHighligther.hpp"
#include "gpu/FigureGeometryCalculator.hpp"
#include "Helpers.hpp"

RectangleHighligther::RectangleHighligther(const uint16_t numberOfRectangles, const std::vector<FrequencyRange> &horizontalRectanglesFrequencyRanges) :
    numberOfRectangles(numberOfRectangles),
    horizontalRectanglesBoundaries(FigureGeometryCalculator::horizontalRectanglesBoundariesFactory(numberOfRectangles)),
    horizontalRectanglesFrequencyRanges(assignFrequencyRangeForAllRectangles(numberOfRectangles, horizontalRectanglesFrequencyRanges))
{
}

//Special case for StereoRmsMeter: two rectangles representing left and right stereo channels
std::vector<FrequencyRange> RectangleHighligther::assignFrequencyRangeForAllRectangles(const uint16_t numberOfRectangles, const std::vector<FrequencyRange> &horizontalRectanglesFrequencyRanges)
{
    std::vector<FrequencyRange> frequencyRanges(horizontalRectanglesFrequencyRanges);

    if(numberOfRectangles > horizontalRectanglesFrequencyRanges.size())
    {
        for(uint32_t i= horizontalRectanglesFrequencyRanges.size(); i<numberOfRectangles ; ++i)
        {
            frequencyRanges.push_back(horizontalRectanglesFrequencyRanges.back());
        }
    }

    return frequencyRanges;
}

RectangleHighligther::HighlightData RectangleHighligther::getData(const WindowSize &windowSize, const CursorPosition& cursorPosition)
{
    HighlightData highlightData{};

    if(isMouseActive(cursorPosition))
    {

        for(uint32_t i=0;i<numberOfRectangles;++i)
        {
            auto horizontalRectangleBoundaries = horizontalRectanglesBoundaries.at(i);

            if(isMouseLocatedOverCurrentlyBeingUsedRectangle(windowSize, cursorPosition, horizontalRectangleBoundaries.first, horizontalRectangleBoundaries.second))
            {
                highlightData.current.index = i;
                highlightData.current.boundaries = horizontalRectangleBoundaries;
                highlightData.current.frequencyRange = horizontalRectanglesFrequencyRanges.at(i);
                highlightData.current.state = true;
            }
        }
    }

    highlightData.previous = previous;
    previous = highlightData.current;

    return highlightData;
}

std::string RectangleHighligther::getStringToBePrinted(const FrequencyRange frequencyRange, const float averagedDBFs, const float maxHoldDbFs, const float peakDbFs)
{
    const uint32_t freqStart = frequencyRange.first;
    const uint32_t stopStop = frequencyRange.second;

    const std::string singleValue = "freq: "+std::to_string(freqStart)+" Hz";
    const std::string range = "f: ["+std::to_string(freqStart)+"-"+std::to_string(stopStop)+"] Hz";

    const std::string freq((freqStart == stopStop) ? singleValue : range);
    const std::string avr("avr: "+formatFloat(averagedDBFs,4,1)+"dBFs");
    const std::string max("max: "+formatFloat(maxHoldDbFs,4,1)+"dBFs");
    const std::string peak("peak: "+formatFloat(peakDbFs,4,1)+"dBFs");

    return (freq+"\n"+avr+"\n"+max+"\n"+peak);
}

bool RectangleHighligther::isMouseLocatedOverCurrentlyBeingUsedRectangle(const WindowSize& windowSize, const CursorPosition& cursorPosition, const double xRectangleBegin, const double xRectangleEnd)
{
    auto scalledMousePosition = 2*(cursorPosition.x/ windowSize.x)-1;

    return ((scalledMousePosition > xRectangleBegin) and (scalledMousePosition< xRectangleEnd)) ? true : false;
}

bool RectangleHighligther::isMouseActive(const CursorPosition& cursorPosition)
{
    double xMouseMove = cursorPosition.x - previousCursorPosition.x;
    double yMouseMove = cursorPosition.y - previousCursorPosition.y;

    auto mouseMove = static_cast<float>(std::sqrt(xMouseMove * xMouseMove + yMouseMove * yMouseMove));

    previousCursorPosition = cursorPosition;

    if (mouseMove > someSmallMouseMoveThreshold)
    {
        lastMouseMoveTime = std::chrono::steady_clock::now();
        return true;
    }
    else if (duration_cast<milliseconds>(std::chrono::steady_clock::now() - lastMouseMoveTime).count() > mouseNotActiveThresholdInMs)
    {
        return false;
    }
    return true;
}


