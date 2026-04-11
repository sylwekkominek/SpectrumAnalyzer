/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "CommonTypes.hpp"
#include <vector>
#include <chrono>
#include <string>

using namespace std::chrono;

class RectangleHighligther
{
public:

    using Index = uint16_t;
    using State = bool;
    using Boundaries = std::pair<float, float>;
    using Center = float;

    struct Data
    {
        Index index;
        State state;
        Boundaries boundaries;
        FrequencyRange frequencyRange;
    };

    struct HighlightData
    {
        Data current;
        Data previous;
    };

    RectangleHighligther(const uint16_t numberOfRectangles, const std::vector<FrequencyRange> &horizontalRectanglesFrequencyRanges);
    HighlightData getData(const WindowSize &windowSize, const CursorPosition& cursorPosition);
    std::string getStringToBePrinted(const FrequencyRange frequencyRange, const float averagedDBFs, const float maxHoldDbFs, const float peakDbFs);
private:

    bool isMouseLocatedOverCurrentlyBeingUsedRectangle(const WindowSize& windowSize, const CursorPosition& cursorPosition, const double xRectangleBegin, const double xRectangleEnd);
    bool isMouseActive(const CursorPosition& cursorPosition);
    std::vector<FrequencyRange> assignFrequencyRangeForAllRectangles(const uint16_t numberOfRectangles, const std::vector<FrequencyRange> &horizontalRectanglesFrequencyRanges);

    Data previous{};

    const uint16_t numberOfRectangles;
    time_point<steady_clock> lastMouseMoveTime;
    CursorPosition previousCursorPosition;
    const std::vector<Boundaries> horizontalRectanglesBoundaries;
    const std::vector<FrequencyRange> horizontalRectanglesFrequencyRanges;
    static constexpr uint32_t mouseNotActiveThresholdInMs = 10000;
    static constexpr double someSmallMouseMoveThreshold = 3;

};
