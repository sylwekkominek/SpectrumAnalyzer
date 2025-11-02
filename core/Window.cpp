/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Window.hpp"
#include "DataSelector.hpp"
#include "Helpers.hpp"
#include "RectangleHighligther.hpp"
#include "DynamicMaxHolder.hpp"
#include "gpu/FigureGeometryCalculator.hpp"

Window::Window(const Configuration &config, const bool isFullScreenEnabled) :
    WindowBase(config, isFullScreenEnabled)
{

    anyData.add(DataSelector(config.samplingRate, config.numberOfSamples, config.frequencies));
    anyData.add(Averager(config.desiredFrameRate));
    anyData.add(time_point<steady_clock>(steady_clock::now()));
    anyData.add(FigureGeometryCalculator::getHorizontalLines(scaleDbfsToPercents(moveDbFsToPositiveValues(config.horizontalLinePositions))));
    anyData.add(DynamicMaxHolder(config.numberOfRectangles, config.dynamicMaxHoldSpeedOfFalling, config.dynamicMaxHoldAccelerationStateOfFalling));
    anyData.add(RectangleHighligther(config.numberOfRectangles));

    createWindow();
}

void Window::initializeGPU()
{
    gpu.init();
    gpu.enableTransparency();
    gpu.prepareBackground(config.backgroundColorSettings);
    gpu.prepareRectangles(config.numberOfRectangles, config.gapWidthInRelationToRectangleWidth, config.colorsOfRectangle, config.advancedColorSettings);
    gpu.prepareDynamicMaxHoldRectangles(config.numberOfRectangles, config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize, config.gapWidthInRelationToRectangleWidth, config.colorsOfDynamicMaxHoldRectangle);
    gpu.prepareHorizontalLines(config.horizontalLinePositions.size());
    gpu.prepareStaticTexts(config.horizontalLinePositions, config.colorOfStaticLines);
    gpu.prepareDynamicText();

    operations.emplace_back("prepareData", [&](){

        auto dBFsValues = anyData.get<DataSelector>()(anyData.get<std::vector<float>>());
        anyData.get<DynamicMaxHolder>().calculate(dBFsValues);
        anyData.add(getWindowSize());
    });

    operations.emplace_back("clear", [&](){
        gpu.clear();
    });

    operations.emplace_back("updateTime", [&](){
        auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - anyData.get<time_point<steady_clock>>()).count();
        gpu.updateTime(timeInMs);
    });

    operations.emplace_back("background", [&](){
        gpu.drawBackground();
    });

    operations.emplace_back("horizontalLines", [&](){
        gpu.drawHorizontalLines(anyData.get<std::vector<Line>>(), config.colorOfStaticLines);
    });

    operations.emplace_back("staticTexts", [&](){

        gpu.drawStaticTexts(anyData.get<std::vector<Line>>(), anyData.get<WindowSize>());
    });

    operations.emplace_back("rectangles", [&](){
        auto positions = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DataSelector>()()));
        gpu.drawRectangles(positions);
    });

    operations.emplace_back("dynamicMaxHold", [&](){
        const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DynamicMaxHolder>().get()));
        gpu.drawDynamicMaxHoldRectangles(dynamicMaxHoldElementsPosition);
    });

    operations.emplace_back("highlight", [&](){

        CursorPosition cursorPosition = getCursorPosition();
        auto windowSize = anyData.get<WindowSize>();
        auto &rectangleHighligther = anyData.get<RectangleHighligther>();
        auto hightlightData = rectangleHighligther.getData(windowSize, cursorPosition);

        if(hightlightData.current.state)
        {
            const auto averagedDBfs = anyData.get<Averager>()(anyData.get<DataSelector>()().at(hightlightData.current.index));

            if(averagedDBfs)
            {
                const auto dBFsHighlightedValues = rectangleHighligther.getStringToBePrinted(config.frequencies.at(hightlightData.current.index),averagedDBfs.value() , anyData.get<DynamicMaxHolder>().get().at(hightlightData.current.index));
                gpu.drawText(dBFsHighlightedValues, (cursorPosition.x > windowSize.x -128) ? HorizontalAligment::RIGHT : HorizontalAligment::LEFT , cursorPosition.x, cursorPosition.y);
            }

            gpu.updateHorizontalRectangleBoundaries(hightlightData.current.index, hightlightData.current.boundaries.first, hightlightData.current.boundaries.second);
        }
        else if(hightlightData.previous.state && not hightlightData.current.state)
        {
            gpu.updateHorizontalRectangleBoundaries(hightlightData.previous.index, 0, 0);
        }

    });

}

void Window::draw(const std::vector<float> &data)
{
    anyData.add(data);

    for(auto &[name, operation]:operations)
    {
        operation();
    }

    swapBuffers();
}

Window::~Window()
{
}
