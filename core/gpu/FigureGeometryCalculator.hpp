/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "ElementInsideGpu.hpp"
#include "CommonTypes.hpp"
#include <vector>

struct FigureGeometryCalculator
{
    struct HorizontalLines
    {
        Lines lines;
    };

    struct VerticalLines
    {
        Lines lines;
    };

    struct VerticalLineTextPositions
    {
        Positions positions;
    };


    static std::vector<std::pair<float, float>> horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles, const float gap=0, const float xDrawOffsetInPercents=FigureGeometryCalculator::xDrawOffsetInPercents, const float xDrawSizeInPercents=FigureGeometryCalculator::xDrawSizeInPercents);
    static Rectangles rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize=0, const float gap=0, const float xDrawOffsetInPercents=FigureGeometryCalculator::xDrawOffsetInPercents, const float xDrawSizeInPercents=FigureGeometryCalculator::xDrawSizeInPercents);
    static HorizontalLines getHorizontalLines(const Positions &positions);
    static VerticalLines getVerticalLines(const uint16_t numberOfRectangles, const std::vector<uint16_t> &indexes);
    static VerticalLineTextPositions getVerticalLineTextPositions(const uint16_t numberOfRectangles, const std::vector<uint16_t> &indexes);
    static Lines getDynamicLines(const std::vector<float> &dataToBePrinted);
    static float getWidthFromWidthInPercents(const float widthInPercents);
    static float getOffsetFromOffsetInPercents(const float offsetInPercents);

    static float xDrawOffsetInPercents;
    static float xDrawSizeInPercents;
};


