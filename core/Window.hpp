/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "ElementsInsideGpu.hpp"
#include "ConfigReader.hpp"
#include "IndexSelector.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>

using namespace std::chrono;

class Window
{
public:
    Window(const Configuration &config, const bool fullScreenEnabled);
    void initializeGPU();
    void draw(const std::vector<float> &data);
    bool checkIfWindowShouldBeClosed();
    bool checkIfWindowShouldBeRecreated();
    ~Window();

private:

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    std::vector<Rectangle> rectanglesFactory(const float heightInPercentOfScreenSize, const float offsetInPercentOffScreenSize=0);
    std::vector<Line> getHorizontalLines(const Positions &positions);
    std::vector<float> getPositionsOfDynamicMaxHoldElements(const std::vector<float> &dataToBePrinted);
    std::vector<float> scaleDbfsToPercentsOfTheScreen(const std::vector<float> &signalInDbfs);
    std::vector<float> extractDataToBePrinted(const std::vector<float> &data);
    std::vector<float> moveDbFsToPositiveValues(const std::vector<float> &signalInDbfs);

    const Configuration config;

    IndexSelector indexSelector;

    std::vector<RectangleInsideGpu> rectanglesInsideGpu;
    std::vector<RectangleInsideGpu> dynamicMaxHoldRectanglesInsideGpu;
    std::vector<LineInsideGpu> horizontalLinesInsideGpu;

    std::vector<float> positionOfDynamicMaxHoldElements;
    const time_point<high_resolution_clock> startTime;
    std::vector<time_point<high_resolution_clock>> timesWhenDynamicMaxHoldElementsHaveBeenUpdated;

    const std::vector<Line> horizontalLinePositions;

    GLFWwindow* window;
    const bool isFullScreenEnabled;

    static constexpr float fullScreenSizeInPercents{100};
};

