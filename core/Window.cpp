/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"
#include "CommonData.hpp"
#include "Helpers.hpp"
#include <algorithm>

Window::Window(const Configuration &config, const bool isFullScreenEnabled) :
    config(config),
    indexSelector(config.samplingRate, config.numberOfSamples, config.frequencies),
    positionOfDynamicMaxHoldElements(config.numberOfRectangles),
    startTime(high_resolution_clock::now()),
    timesWhenDynamicMaxHoldElementsHaveBeenUpdated(config.numberOfRectangles, startTime),
    horizontalLinePositions(getHorizontalLines(scaleDbfsToPercentsOfTheScreen(moveDbFsToPositiveValues(config.horizontalLinePositions)))),
    isFullScreenEnabled(isFullScreenEnabled)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if(isFullScreenEnabled)
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        window = glfwCreateWindow(config.maximizedWindowHorizontalSize, config.maximizedWindowVerticalSize, "SpectrumAnalyzer", glfwGetPrimaryMonitor(), nullptr);
        TextInsideGpu::updateWindowSize(config.maximizedWindowHorizontalSize, config.maximizedWindowVerticalSize);
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        window = glfwCreateWindow(config.normalWindowHorizontalSize, config.normalWindowVerticalSize, "SpectrumAnalyzer", nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        TextInsideGpu::updateWindowSize(config.normalWindowHorizontalSize, config.normalWindowVerticalSize);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::initializeGPU()
{

    RectangleInsideGpu<RectangleType::BACKGROUND>::initialize(config.backgroundColorSettings.c_str());
    RectangleInsideGpu<RectangleType::BAR>::initialize(config.advancedColorSettings.c_str());

    LineInsideGpu::initialize();
    TextInsideGpu::initialize();

    backgroundInsideGpu = std::make_unique<RectangleInsideGpu<RectangleType::BACKGROUND>>(rectanglesFactory(100, 1,100).front());

    for(const auto & rectangle : rectanglesFactory(fullScreenSizeInPercents,config.numberOfRectangles))
    {
        rectanglesInsideGpu.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle, config.colorsOfRectangle));
    }

    for(const auto & rectangle : rectanglesFactory(config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize, config.numberOfRectangles, 50 + config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize/2))
    {
        dynamicMaxHoldRectanglesInsideGpu.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle,  config.colorsOfDynamicMaxHoldRectangle));
    }

    for(int i=0;i<config.horizontalLinePositions.size();++i)
    {
        horizontalLinesInsideGpu.emplace_back(LineInsideGpu());
    }

    for(int i=0;i<config.horizontalLinePositions.size();++i)
    {
        textsInsideGpu.emplace_back(formatFloat(config.horizontalLinePositions.at(i),4,2), config.colorOfStaticLines);
    }
}

void Window::draw(const std::vector<float> &data)
{
    auto positions = scaleDbfsToPercentsOfTheScreen(moveDbFsToPositiveValues(extractDataToBePrinted(data)));
    auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - startTime).count();

    RectangleInsideGpu<RectangleType::BAR>::updateTime(timeInMs);
    RectangleInsideGpu<RectangleType::BACKGROUND>::updateTime(timeInMs);

    glClear(GL_COLOR_BUFFER_BIT);

    backgroundInsideGpu->draw();


    for(uint i=0;i<config.horizontalLinePositions.size();++i)
    {
        horizontalLinesInsideGpu.at(i).draw(horizontalLinePositions.at(i), config.colorOfStaticLines);
    }

    for(uint i=0;i<config.horizontalLinePositions.size();++i)
    {
        textsInsideGpu.at(i).draw(horizontalLinePositions.at(i).front().y);
    }

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
    return glfwWindowShouldClose(window);
}

bool Window::checkIfWindowShouldBeRecreated()
{
    glfwPollEvents();

    if((isFullScreenEnabled && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS))
    {
        return true;
    }

    if((not isFullScreenEnabled) && glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
    {
        return true;
    }

    return false;
}

void Window::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
    TextInsideGpu::updateWindowSize(width, height);
}

Window::~Window()
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::finalize();
    RectangleInsideGpu<RectangleType::BAR>::finalize();
    LineInsideGpu::finalize();
    TextInsideGpu::finalize();

    glfwDestroyWindow(window);
}

std::vector<Rectangle> Window::rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize)
{
    const double fullScreenSize = 2.0;
    const double xBeginOfZeroElement = -1;

    const double numberOfGaps = numberOfRectangles -1;
    const double xWidth =  (fullScreenSize/(numberOfRectangles + numberOfGaps * config.gapWidthInRelationToRectangleWidth));

    const float  offset  = (offsetInPercentOffScreenSize/50);

    const float yBegin= (-heightInPercentOfScreenSize/100)+offset;
    const float yEnd = (heightInPercentOfScreenSize/100)+offset;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(numberOfRectangles);

    for(uint i=0;i<numberOfRectangles;++i)
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

std::vector<Line> Window::getHorizontalLines(const Positions &positions)
{
    double xBegin = 3;
    double xEnd = 97;

    std::vector<Line> lines{};
    lines.reserve(positions.size());

    for(const auto & position : positions)
    {
        Line line;

        line.push_back(Point{static_cast<float>(xBegin), static_cast<float>(position)});
        line.push_back(Point{static_cast<float>(xEnd),static_cast<float>(position)});
        lines.emplace_back(std::move(line));
    }

    return lines;
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

std::vector<float> Window::moveDbFsToPositiveValues(const std::vector<float> &signalInDbfs)
{
    std::vector<float> outputData(signalInDbfs.size());

    std::transform(signalInDbfs.begin(), signalInDbfs.end(), outputData.begin(), [](const auto &el){ return el+getDynamicRangeOf16bitSignal(); });
    return outputData;
}

std::vector<float> Window::scaleDbfsToPercentsOfTheScreen(const std::vector<float> &signalInDbfs)
{
    constexpr float scallingFactor = fullScreenSizeInPercents/getDynamicRangeOf16bitSignal();

    std::vector<float> outputData(signalInDbfs.size());

    std::transform(signalInDbfs.begin(), signalInDbfs.end(), outputData.begin(), [&scallingFactor](const auto &el){ return el*scallingFactor; });
    return outputData;
}

std::vector<float> Window::extractDataToBePrinted(const std::vector<float> &data)
{
    std::vector<float> positions(config.numberOfRectangles,0);

    for(uint i=0;i<config.numberOfRectangles;++i)
    {
        positions[i] = data.at(indexSelector.getFrequencyIndex(i));
    }

    return positions;
}



