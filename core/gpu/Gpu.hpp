/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "RectangleInsideGpu.hpp"
#include "LineInsideGpu.hpp"
#include "TextInsideGpu.hpp"
#include "CommonTypes.hpp"
#include <vector>
#include <memory>

struct Gpu
{
    void init();
    void enableTransparency();
    void prepareBackground(const std::string &backgroundConfig);
    void prepareRectangles(const uint16_t numberOfRectangles, const float gap, const ColorsOfRectanglePerVertices &colorsOfRectangle, const std::string &rectanglesConfig);
    void prepareDynamicMaxHoldRectangles(const uint16_t numberOfRectangles, const float height, const float gap, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void prepareHorizontalLines(const uint16_t size);
    void prepareStaticTexts(const Positions &horizontalLinePositions, const Color &colorOfStaticLines);
    void prepareDynamicText();
    void updateTime(const float timeInMilliSeconds);
    void drawBackground();
    void drawHorizontalLines(const std::vector<Line> &horizontalLinePositions, const Color &colorOfStaticLines);
    void drawStaticTexts(const std::vector<Line> &horizontalLinePositions, const WindowSize &windowSize);
    void drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition);
    void drawRectangles(const std::vector<float> &rectaglesPositions);
    void updateHorizontalRectangleBoundaries(const uint16_t indexOfRectangle, const float start, const float stop);
    void drawText(const std::string &str, const HorizontalAligment aligment, const float x, const float y);
    void clear();
    ~Gpu();

private:
    float convertPositionInPercentToPixels(const float positionInPercents, const float screenSize);

    std::unique_ptr<RectangleInsideGpu<RectangleType::BACKGROUND>> background;
    std::vector<RectangleInsideGpu<RectangleType::BAR>> rectangles;
    std::vector<RectangleInsideGpu<RectangleType::BAR>> dynamicMaxHoldRectangles;
    std::vector<LineInsideGpu> horizontalLines;
    std::vector<TextInsideGpu> staticTexts;
    std::unique_ptr<TextInsideGpu> dynamicText;

};

