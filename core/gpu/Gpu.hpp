/*
 * Copyright (C) 2024-2026, Sylwester Kominek
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
    void initLines();
    void initText();
    void initRectangles(const std::string &backgroundConfig, const std::string &rectanglesConfig);
    void enableTransparency();
    void prepareBackground(const Rectangles &rectangles);
    void prepareRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void prepareDynamicMaxHoldRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void prepareDynamicMaxHoldSecondaryRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle);
    void prepareHorizontalLines(const uint16_t size);
    void prepareVerticalLines(const uint16_t size);
    void prepareDynamicLines(const uint16_t size);
    void prepareDynamicMaxHoldLines(const uint16_t size);
    void prepareDynamicMaxHoldSecondaryLines(const uint16_t size);
    void prepareHorizontalLineStaticTexts(const std::vector<float> &dbfsValues, const Color &colorOfStaticLines);
    void prepareVerticalLineStaticTexts(const Frequencies &frequencies, const Color &colorOfStaticLines);
    void prepareDynamicText();
    void updateTime(const float timeInMilliSeconds);
    void updateThemeNumber(const uint16_t themeNumber);
    void drawBackground();
    void drawHorizontalLines(const Lines &horizontalLinePositions, const Color &colorOfStaticLines);
    void drawVerticalLines(const Lines &verticalLinePositions, const Color &colorOfStaticLines);
    void drawDynamicLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines);
    void drawDynamicMaxHoldLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines);
    void drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize, const float xDrawOffsetInPercents, const float xDrawSizeInPercents);
    void drawVerticalLineStaticTexts(const Positions &verticalLineTextPositions, const WindowSize &windowSize);
    void drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition);
    void drawDynamicMaxHoldSecondaryRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition);
    void drawDynamicMaxHoldSecondaryLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines);
    void drawRectangles(const std::vector<float> &rectaglesPositions);
    void updateHorizontalRectangleBoundaries(const uint16_t indexOfRectangle, const float start, const float stop);
    void drawText(const std::string &str, const HorizontalAligment aligment, const float x, const float y);

    void clear();
    ~Gpu();

private:
    float convertXPositionInPercentToPixels(const float positionInPercents, const float xScreenSize);
    float convertYPositionInPercentToPixels(const float positionInPercents, const float yScreenSize);

    std::unique_ptr<RectangleInsideGpu<RectangleType::BACKGROUND>> background;
    std::vector<RectangleInsideGpu<RectangleType::BAR>> rectangles;
    std::vector<RectangleInsideGpu<RectangleType::BAR>> dynamicMaxHoldRectangles;
    std::vector<RectangleInsideGpu<RectangleType::SECONDARY_BAR>> dynamicMaxHoldSecondaryRectangles;
    std::vector<LineInsideGpu> horizontalLines;
    std::vector<LineInsideGpu> verticalLines;
    std::vector<LineInsideGpu> dynamicLines;
    std::vector<LineInsideGpu> dynamicMaxHoldLines;
    std::vector<LineInsideGpu> dynamicMaxHoldSecondaryLines;
    std::vector<TextInsideGpu> horizontalLineStaticTexts;
    std::vector<TextInsideGpu> verticalLineStaticTexts;
    std::unique_ptr<TextInsideGpu> dynamicText;
};

