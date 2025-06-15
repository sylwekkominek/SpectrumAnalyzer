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
    Window(const Configuration &config);
    void initializeGPU();
    void draw(const std::vector<float> &data);
    bool checkIfWindowShouldBeClosed();
    bool checkIfWindowShouldBeRecreated();
    ~Window();

private:

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void windowMaximizeCallback(GLFWwindow* window, int maximized);

    std::vector<Rectangle> rectanglesFactory(const float heightInPercentOfScreenSize, const float offsetInPercentOffScreenSize=0);
    std::vector<float> getPositionsOfDynamicMaxHoldElements(const std::vector<float> &dataToBePrinted);

    const Configuration config;

    IndexSelector indexSelector;

    std::vector<RectangleInsideGpu> rectanglesInsideGpu;
    std::vector<RectangleInsideGpu> dynamicMaxHoldRectanglesInsideGpu;

    std::vector<float> positionOfDynamicMaxHoldElements;
    std::vector<time_point<high_resolution_clock>> timesWhenDynamicMaxHoldElementsHaveBeenUpdated;

    GLFWwindow* window;
    static bool isMaximized;
};

