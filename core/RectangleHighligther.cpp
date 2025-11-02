/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "RectangleHighligther.hpp"
#include "gpu/FigureGeometryCalculator.hpp"
#include "Helpers.hpp"

RectangleHighligther::RectangleHighligther(const uint16_t numberOfRectangles) :
    numberOfRectangles(numberOfRectangles),
    horizontalRectanglesBoundaries(FigureGeometryCalculator::horizontalRectanglesBoundariesFactory(numberOfRectangles))
{
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
                highlightData.current.state = true;
            }
        }
    }

    highlightData.previous = previous;
    previous = highlightData.current;

    return highlightData;
}

std::string RectangleHighligther::getStringToBePrinted(const float frequency, const float averagedDBFs, const float maxHoldDbFs)
{

    const std::string freq("freq: "+formatFloat(frequency,4,0)+"Hz");
    const std::string avrPwr("avr pwr: "+formatFloat(averagedDBFs,4,1)+"dBFs");
    const std::string maxPwr("max pwr: "+formatFloat(maxHoldDbFs,4,1)+"dBFs");

    return (freq+"\n"+avrPwr+"\n"+maxPwr);
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


