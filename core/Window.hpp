/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "gpu/LineInsideGpu.hpp"
#include "gpu/RectangleInsideGpu.hpp"
#include "gpu/TextInsideGpu.hpp"
#include "ConfigReader.hpp"
#include "IndexSelector.hpp"
#include "DataCalculator.hpp"
#include "WindowBase.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <memory>
#include <optional>

using namespace std::chrono;


class Window : public WindowBase
{
public:
    Window(const Configuration &config, const bool fullScreenEnabled);
    void initializeGPU();
    void draw(const std::vector<float> &data);
    ~Window();

private:

    std::vector<std::pair<float, float>> horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles);
    std::vector<Rectangle> rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize=0);
    std::vector<Line> getHorizontalLines(const Positions &positions);
    std::vector<float> calculateDynamicMaxHoldValues(const std::vector<float> &dataToBePrinted);
    std::vector<float> scaleDbfsToPercentsOfTheScreen(const std::vector<float> &signalInDbfs);
    std::vector<float> extractDataToBePrinted(const std::vector<float> &data);
    std::vector<float> moveDbFsToPositiveValues(const std::vector<float> &signalInDbfs);
    std::string getHighlightStringToBePrinted(const float frequency, const float dBFs, const float maxHoldDbFs);

    bool isMouseActive(const CursorPosition& cursorPosition);
    bool isMouseLocatedOverCurrentlyBeingUsedRectangle(const WindowSize& windowSize, const CursorPosition& cursorPosition, const double xRectangleBegin, const double xRectangleEnd);
    std::optional<uint16_t> getRectangleIndexOverWhichMouseIsActive(const WindowSize& windowSize, const CursorPosition& cursorPosition);
    float convertPositionInPercentToPixels(const float positionInPercents, const float screenSize);

    IndexSelector indexSelector;
    DataAverager dataAverager{1,(uint32_t)config.desiredFrameRate};

    std::vector<RectangleInsideGpu<RectangleType::BAR>> rectanglesInsideGpu;
    std::vector<RectangleInsideGpu<RectangleType::BAR>> dynamicMaxHoldRectanglesInsideGpu;
    std::unique_ptr<RectangleInsideGpu<RectangleType::BACKGROUND>> backgroundInsideGpu;

    std::vector<LineInsideGpu> horizontalLinesInsideGpu;
    std::vector<TextInsideGpu> staticTextsInsideGpu;
    std::unique_ptr<TextInsideGpu> currentMarkedBarText;

    std::vector<float> dynamicMaxHoldValues;
    const time_point<steady_clock> startTime;
    std::vector<time_point<steady_clock>> timesWhenDynamicMaxHoldValuesHaveBeenUpdated;

    const std::vector<Line> horizontalLinePositions;
    const std::vector<std::pair<float, float>> horizontalRectanglesBoundaries;

    std::optional<uint16_t> previousIndexOfRectangleOverWhichMouseIsActive;
};

