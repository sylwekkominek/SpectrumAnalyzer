/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "RectanglesInsideGpu.hpp"
#include "LinesInsideGpu.hpp"
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
    void prepareHorizontalLines(const uint16_t size, const Color &colorOfStaticLines);
    void prepareVerticalLines(const uint16_t size, const Color &colorOfStaticLines);
    void prepareDynamicLines(const uint16_t size, const Color &colorOfDynamicLines);
    void prepareDynamicMaxHoldLines(const uint16_t size, const Color &colorOfDynamicLines);
    void prepareDynamicMaxHoldSecondaryLines(const uint16_t size, const Color &colorOfDynamicLines);
    void prepareHorizontalLineStaticTexts(const std::vector<float> &dbfsValues, const Color &colorOfStaticLines);
    void prepareVerticalLineStaticTexts(const Frequencies &frequencies, const Color &colorOfStaticLines);
    void prepareHighlightedVerticalLine(const Color &color);
    void prepareDynamicText();
    void updateTime(const float timeInMilliSeconds);
    void drawBackground();
    void drawHorizontalLines(const Lines &horizontalLinePositions);
    void drawVerticalLines(const Lines &verticalLinePositions);
    void drawDynamicLines(const Lines &dynamicLinePositions);
    void drawDynamicMaxHoldLines(const Lines &dynamicLinePositions);
    void drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize, const float xDrawOffsetInPercents, const float xDrawSizeInPercents);
    void drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize);
    void drawVerticalLineStaticTexts(const Positions &verticalLineTextPositions, const WindowSize &windowSize);
    void drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition);
    void drawDynamicMaxHoldSecondaryRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition);
    void drawDynamicMaxHoldSecondaryLines(const Lines &dynamicLinePositions);
    void drawRectangles(const std::vector<float> &rectaglesPositions);
    void updateHorizontalRectangleBoundaries(const uint16_t indexOfRectangle, const float start, const float stop);
    void drawHighlightedVerticalLine(const Line &line);
    void drawText(const std::string &str, const HorizontalAligment aligment, const float x, const float y);

    void clear();
    ~Gpu();

private:
    float convertXPositionInPercentToPixels(const float positionInPercents, const float xScreenSize);
    float convertYPositionInPercentToPixels(const float positionInPercents, const float yScreenSize);

    std::unique_ptr<RectanglesInsideGpu<RectangleType::BACKGROUND>> background;
    std::unique_ptr<RectanglesInsideGpu<RectangleType::BAR>> rectangles;
    std::unique_ptr<RectanglesInsideGpu<RectangleType::BAR>> dynamicMaxHoldRectangles;
    std::unique_ptr<RectanglesInsideGpu<RectangleType::SECONDARY_BAR>> dynamicMaxHoldSecondaryRectangles;
    std::unique_ptr<LinesInsideGpu> horizontalLines;
    std::unique_ptr<LinesInsideGpu> verticalLines;
    std::unique_ptr<LinesInsideGpu> dynamicLines;
    std::unique_ptr<LinesInsideGpu> dynamicMaxHoldLines;
    std::unique_ptr<LinesInsideGpu> dynamicMaxHoldSecondaryLines;
    std::vector<TextInsideGpu> horizontalLineStaticTexts;
    std::vector<TextInsideGpu> verticalLineStaticTexts;
    std::unique_ptr<TextInsideGpu> dynamicText;
    std::unique_ptr<LinesInsideGpu> highlightedVerticalLine;
};

