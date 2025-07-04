/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"

bool Window::isMaximized = false;

Window::Window(const Configuration &config) :
    config(config),
    indexSelector(config.samplingRate, config.numberOfSamples, config.frequencies),
    positionOfDynamicMaxHoldElements(config.numberOfRectangles),
    timesWhenDynamicMaxHoldElementsHaveBeenUpdated(config.numberOfRectangles, high_resolution_clock::now())
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    window = glfwCreateWindow(config.horizontalSize, config.verticalSize, "SpectrumAnalyzer", nullptr, nullptr);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(window, windowMaximizeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();
}

void Window::initializeGPU()
{
    const float fullScreenSizeInPercents{100};

    RectangleInsideGpu::initialize(config.advancedColorSettings.c_str());

    for(const auto & rectangle : rectanglesFactory(fullScreenSizeInPercents))
    {
            rectanglesInsideGpu.emplace_back(RectangleInsideGpu(rectangle, config.colorsOfRectangle));
    }

    for(const auto & rectangle : rectanglesFactory(config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize, 50))
    {
        dynamicMaxHoldRectanglesInsideGpu.emplace_back(RectangleInsideGpu(rectangle,  config.colorsOfDynamicMaxHoldRectangle));
    }
}

void Window::draw(const std::vector<float> &data)
{
    std::vector<float> positions(config.numberOfRectangles,0);

    for(uint i=0;i<config.numberOfRectangles;++i)
    {
        positions[i] = config.scalingFactor*(data.at(indexSelector.getFrequencyIndex(i))+config.offsetFactor);
    }

    glClear(GL_COLOR_BUFFER_BIT);


    for(uint i=0;i<positions.size();++i)
    {
        rectanglesInsideGpu.at(i).move(positions.at(i));
    }

    if( config.dynamicMaxHoldVisibilityState)
    {
        const auto dynamicMaxHoldElementsPosition = getPositionsOfDynamicMaxHoldElements(positions);

        for(uint i=0;i<dynamicMaxHoldRectanglesInsideGpu.size();++i)
        {
            dynamicMaxHoldRectanglesInsideGpu.at(i).move(dynamicMaxHoldElementsPosition.at(i));
        }
    }

    glfwSwapBuffers(window);
}

bool Window::checkIfWindowShouldBeClosed()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(window))
    {
        return true;
    }

    return false;
}

bool Window::checkIfWindowShouldBeRecreated()
{
    glfwPollEvents();

    return ((isMaximized && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))  ? (isMaximized = false, true) : false;
}

void Window::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::windowMaximizeCallback(GLFWwindow* glfwWindow, int maximized)
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    isMaximized = (maximized == GLFW_TRUE);
}

Window::~Window()
{
    RectangleInsideGpu::finalize();
    glfwDestroyWindow(window);
}

std::vector<Rectangle> Window::rectanglesFactory(const float heightInPercentOfScreenSize, const float offsetInPercentOffScreenSize)
{
    const double fullScreenSize = 2.0;
    const double xBeginOfZeroElement = -1;
    const double numberOfGaps = config.numberOfRectangles -1;
    const double xWidth =  (fullScreenSize/(config.numberOfRectangles + numberOfGaps * config.gapWidthInRelationToRectangleWidth));


    const float  offset  = (offsetInPercentOffScreenSize/50);

    const float yBegin= (-heightInPercentOfScreenSize/100)+offset;
    const float yEnd = (heightInPercentOfScreenSize/100)+offset;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(config.numberOfRectangles);

    for(uint i=0;i<config.numberOfRectangles;++i)
    {
        double xBegin =  xBeginOfZeroElement + i*xWidth*(1.0 + config.gapWidthInRelationToRectangleWidth);
        double xEnd = xBegin + xWidth;


        /*
        Each rectangle consists of 2 triangles

        3______5   2
        |     /   /|
        |    /   / |
        |   /   /  |
        |  /   /   |
        | /   /    |
        |/   /_____|
        4   0      1
        */

        Rectangle rectangle;
        rectangles.reserve(6);

        rectangle.push_back(Point{static_cast<float>(xEnd), static_cast<float>(yEnd)});         //2
        rectangle.push_back(Point{static_cast<float>(xEnd), static_cast<float>(yBegin)});       //1
        rectangle.push_back(Point{static_cast<float>(xBegin), static_cast<float>(yBegin)});     //0

        rectangle.push_back(Point{static_cast<float>(xEnd), static_cast<float>(yEnd)});         //5
        rectangle.push_back(Point{static_cast<float>(xBegin), static_cast<float>(yBegin)});     //4
        rectangle.push_back(Point{static_cast<float>(xBegin), static_cast<float>(yEnd)});       //3

        rectangles.emplace_back(std::move(rectangle));
    }

    return rectangles;
}


std::vector<float> Window::getPositionsOfDynamicMaxHoldElements(const std::vector<float> &dataToBePrinted)
{
    auto time = high_resolution_clock::now();

    for(uint i=0;i<config.numberOfRectangles;++i)
    {
        uint32_t diffInTime = (duration_cast<milliseconds>(time - timesWhenDynamicMaxHoldElementsHaveBeenUpdated[i])).count();

        float diffInPosition = diffInTime/config.dynamicMaxHoldSpeedOfFalling;
        float newPosition = (positionOfDynamicMaxHoldElements[i] - diffInPosition);
        newPosition = newPosition > 0 ?  newPosition : 0;

        if(dataToBePrinted[i] > newPosition)
        {
            positionOfDynamicMaxHoldElements[i] = dataToBePrinted[i];
            timesWhenDynamicMaxHoldElementsHaveBeenUpdated[i] = high_resolution_clock::now();
        }
        else
        {
            positionOfDynamicMaxHoldElements[i] =  newPosition;
            timesWhenDynamicMaxHoldElementsHaveBeenUpdated[i] = config.dynamicMaxHoldAccelerationStateOfFalling ?  timesWhenDynamicMaxHoldElementsHaveBeenUpdated[i]: time;
        }
    }

    return positionOfDynamicMaxHoldElements;
}
