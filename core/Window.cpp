/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"
#include "CommonData.hpp"
#include "Helpers.hpp"
#include <algorithm>

std::atomic<uint16_t> Window::xCurrentWindowSize =0;
std::atomic<uint16_t> Window::yCurrentWindowSize =0;


Window::Window(const Configuration &config, const bool isFullScreenEnabled) :
    config(config),
    indexSelector(config.samplingRate, config.numberOfSamples, config.frequencies),
    dynamicMaxHoldValues(config.numberOfRectangles, getFloorDbFs16bit()),
    startTime(steady_clock::now()),
    timesWhenDynamicMaxHoldValuesHaveBeenUpdated(config.numberOfRectangles, startTime),
    horizontalLinePositions(getHorizontalLines(scaleDbfsToPercentsOfTheScreen(moveDbFsToPositiveValues(config.horizontalLinePositions)))),
    horizontalRectanglesBoundaries(horizontalRectanglesBoundariesFactory(config.numberOfRectangles)),
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
        updateCurrentWindowSize(config.maximizedWindowHorizontalSize, config.maximizedWindowVerticalSize);
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        window = glfwCreateWindow(config.normalWindowHorizontalSize, config.normalWindowVerticalSize, "SpectrumAnalyzer", nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        updateCurrentWindowSize(config.normalWindowHorizontalSize, config.normalWindowVerticalSize);
    }

    glfwSetCursorEnterCallback(window, cursorEnteredAreaOverWindowCallback);

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
        staticTextsInsideGpu.emplace_back(formatFloat(config.horizontalLinePositions.at(i),4,2), config.colorOfStaticLines);
    }

    currentMarkedBarText = std::make_unique<TextInsideGpu>("", std::vector<float>{1.0,0,0,1});
}

void Window::draw(const std::vector<float> &data)
{
    auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - startTime).count();

    RectangleInsideGpu<RectangleType::BAR>::updateTime(timeInMs);
    RectangleInsideGpu<RectangleType::BACKGROUND>::updateTime(timeInMs);

    glClear(GL_COLOR_BUFFER_BIT);

    backgroundInsideGpu->draw();

    for(uint32_t i=0;i<config.horizontalLinePositions.size();++i)
    {
        horizontalLinesInsideGpu.at(i).draw(horizontalLinePositions.at(i), config.colorOfStaticLines);
    }

    for(uint32_t i=0;i<config.horizontalLinePositions.size();++i)
    {
        const auto textPositionInPixels = convertPositionInPercentToPixels(horizontalLinePositions.at(i).front().y, yCurrentWindowSize);
        staticTextsInsideGpu.at(i).draw(HorizontalAligment::LEFT, 0,textPositionInPixels);
        staticTextsInsideGpu.at(i).draw(HorizontalAligment::RIGHT, xCurrentWindowSize, textPositionInPixels);
    }

    const auto dBFsValues = extractDataToBePrinted(data);
    const auto dynamicMaxHoldDbFsValue = calculateDynamicMaxHoldValues(dBFsValues);

    if( config.dynamicMaxHoldVisibilityState)
    {
        const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercentsOfTheScreen(moveDbFsToPositiveValues(dynamicMaxHoldDbFsValue));

        for(uint32_t i=0;i<dynamicMaxHoldRectanglesInsideGpu.size();++i)
        {
            dynamicMaxHoldRectanglesInsideGpu.at(i).move(dynamicMaxHoldElementsPosition.at(i));
        }
    }

    const auto positions = scaleDbfsToPercentsOfTheScreen(moveDbFsToPositiveValues(dBFsValues));

    for(uint32_t i=0;i<positions.size();++i)
    {
        rectanglesInsideGpu.at(i).move(positions.at(i));
    }

    double xpos{0};
    double ypos{0};

    glfwGetCursorPos(window, &xpos, &ypos);

    auto indexOfRectangle = getRectangleIndexOverWhichMouseIsActive(xpos);

    if(indexOfRectangle)
    {
        const auto dBFsHighlightedValues = getHighlightStringToBePrinted(config.frequencies.at(indexOfRectangle.value()), dBFsValues.at(indexOfRectangle.value()), dynamicMaxHoldDbFsValue.at(indexOfRectangle.value()));
        auto horizontalRectangleBoundaries = horizontalRectanglesBoundaries.at(indexOfRectangle.value());

        rectanglesInsideGpu.at(indexOfRectangle.value()).updateBoundary(horizontalRectangleBoundaries.first, horizontalRectangleBoundaries.second);
        currentMarkedBarText->draw(dBFsHighlightedValues, (xpos > xCurrentWindowSize-128) ? HorizontalAligment::RIGHT : HorizontalAligment::LEFT , xpos,ypos);
    }
    else if(previousIndexOfRectangleOverWhichMouseIsActive && not indexOfRectangle)
    {
        rectanglesInsideGpu.at(previousIndexOfRectangleOverWhichMouseIsActive.value()).updateBoundary(0, 0);
    }

    previousIndexOfRectangleOverWhichMouseIsActive = indexOfRectangle;

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
    Window::updateCurrentWindowSize(width, height);
}

void Window::cursorEnteredAreaOverWindowCallback(GLFWwindow* window, int entered)
{
    glfwSetInputMode(window, GLFW_CURSOR, (entered ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL));
}

Window::~Window()
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::finalize();
    RectangleInsideGpu<RectangleType::BAR>::finalize();
    LineInsideGpu::finalize();
    TextInsideGpu::finalize();

    glfwDestroyWindow(window);
}

std::vector<std::pair<float, float>> Window::horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles)
{
    const double fullScreenSize = 2.0;
    const double xBeginOfZeroElement = -1;
    const double numberOfGaps = numberOfRectangles -1;
    const double xWidth =  (fullScreenSize/(numberOfRectangles + numberOfGaps * config.gapWidthInRelationToRectangleWidth));

    std::vector<std::pair<float, float>> rectangles;
    rectangles.reserve(numberOfRectangles);

    for(uint32_t i=0;i<numberOfRectangles;++i)
    {
        double xBegin =  xBeginOfZeroElement + i*xWidth*(1.0 + config.gapWidthInRelationToRectangleWidth);
        double xEnd = xBegin + xWidth;
        rectangles.emplace_back(std::make_pair(xBegin, xEnd));
    }

    return rectangles;
}

std::vector<Rectangle> Window::rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize)
{
    const auto xBoundaries = horizontalRectanglesBoundariesFactory(numberOfRectangles);

    const double numberOfGaps = numberOfRectangles -1;
    const float  offset  = (offsetInPercentOffScreenSize/50);
    const float yBegin= (-heightInPercentOfScreenSize/100)+offset;
    const float yEnd = (heightInPercentOfScreenSize/100)+offset;

    std::vector<Rectangle> rectangles;
    rectangles.reserve(numberOfRectangles);

    for(uint32_t i=0;i<numberOfRectangles;++i)
    {
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

        const float xBegin = xBoundaries.at(i).first;
        const float xEnd = xBoundaries.at(i).second;

        rectangle.push_back(Point{static_cast<float>(xBegin), static_cast<float>(yBegin)});     //0
        rectangle.push_back(Point{static_cast<float>(xEnd), static_cast<float>(yBegin)});       //1
        rectangle.push_back(Point{static_cast<float>(xEnd), static_cast<float>(yEnd)});         //2

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

std::vector<float> Window::calculateDynamicMaxHoldValues(const std::vector<float> &dBFs)
{
    auto time = steady_clock::now();

    for(uint32_t i=0;i<config.numberOfRectangles;++i)
    {
        uint32_t diffInTime = (duration_cast<milliseconds>(time - timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i])).count();

        float diffInValue = diffInTime/config.dynamicMaxHoldSpeedOfFalling;

        float newValue = (dynamicMaxHoldValues[i] - diffInValue);

        newValue = newValue > getFloorDbFs16bit() ?  newValue : getFloorDbFs16bit();

        if(dBFs[i] > newValue)
        {
            dynamicMaxHoldValues[i] = dBFs[i];
            timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i] = steady_clock::now();
        }
        else
        {
            dynamicMaxHoldValues[i] =  newValue;
            timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i] = config.dynamicMaxHoldAccelerationStateOfFalling ?  timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i]: time;
        }
    }

    return dynamicMaxHoldValues;
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

    for(uint32_t i=0;i<config.numberOfRectangles;++i)
    {
        positions[i] = data.at(indexSelector.getFrequencyIndex(i));
    }

    return positions;
}

void Window::updateCurrentWindowSize(const uint16_t x, const uint16_t y)
{
    xCurrentWindowSize = x;
    yCurrentWindowSize = y;
}

bool Window::isMouseActive(const double xMousePos, const double yMousePos)
{
    static constexpr uint32_t mouseNotActiveThresholdInMs = 5000;
    static constexpr double someSmallMouseMoveThreshold = 3;
    static time_point<steady_clock> lastMouseMoveTime;
    static double previousMouseXPosition;
    static double previousMouseYPosition;

    const double xMouseMove = (xMousePos - previousMouseXPosition);
    const double yMouseMove = (yMousePos - previousMouseYPosition);
    const double mouseMove = std::sqrt(xMouseMove*xMouseMove + yMouseMove*yMouseMove);

    previousMouseXPosition = xMousePos;
    previousMouseYPosition = yMousePos;

    if(mouseMove > someSmallMouseMoveThreshold)
    {
        lastMouseMoveTime = std::chrono::steady_clock::now();
        return true;
    }
    else if(duration_cast<milliseconds>(std::chrono::steady_clock::now() - lastMouseMoveTime).count() > mouseNotActiveThresholdInMs)
    {
        return false;
    }
    return true;
}

bool Window::isMouseLocatedOverCurrentlyBeingUsedRectangle(const double xMousePos, const double xRectangleBegin, const double xRectangleEnd)
{
    auto scalledMousePosition = 2*(xMousePos/xCurrentWindowSize)-1;

    return ((scalledMousePosition > xRectangleBegin) and (scalledMousePosition< xRectangleEnd)) ? true : false;
}

float Window::convertPositionInPercentToPixels(const float positionInPercents, const float screenSize)
{
    auto position = (100-positionInPercents)/100;
    return position * screenSize;
}

std::optional<uint16_t> Window::getRectangleIndexOverWhichMouseIsActive(const double xMousePos)
{
    if(isMouseActive(xMousePos, 0))
    {
        for(uint32_t i=0;i<config.numberOfRectangles;++i)
        {
            auto horizontalRectangleBoundaries = horizontalRectanglesBoundaries.at(i);

            if(isMouseLocatedOverCurrentlyBeingUsedRectangle(xMousePos, horizontalRectangleBoundaries.first, horizontalRectangleBoundaries.second))
            {
                return i;
            }
        }
    }

    return std::nullopt;
}

std::string Window::getHighlightStringToBePrinted(const float frequency, const float dBFs, const float maxHoldDbFs)
{
    dataAverager.push_back({dBFs});

    auto averagedData = dataAverager.calculate();

    if(not averagedData.empty())
    {
        const std::string freq("freq: "+formatFloat(frequency,4,0)+"Hz");
        const std::string avrPwr("avr pwr: "+formatFloat(averagedData.front(),4,1)+"dBFs");
        const std::string maxPwr("max pwr: "+formatFloat(maxHoldDbFs,4,1)+"dBFs");

        return (freq+"\n"+avrPwr+"\n"+maxPwr);
    }
    return {};
}
