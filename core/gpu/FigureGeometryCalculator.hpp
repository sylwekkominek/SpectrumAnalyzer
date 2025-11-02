/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "ElementInsideGpu.hpp"
#include "ConfigReader.hpp"
#include <vector>

struct FigureGeometryCalculator
{
    static std::vector<std::pair<float, float>> horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles, const float gap=0);
    static std::vector<Rectangle> rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize=0, const float gap=0);
    static std::vector<Line> getHorizontalLines(const Positions &positions);
};
