/*
 * Copyright (C) 2024-2026, Sylwester Kominek
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

    anyData.add(DataSelector(config.get<SamplingRate>(), config.get<NumberOfSamples>(), config.get<Freqs>()));
    anyData.add(Averager(config.get<DesiredFrameRate>()));
    anyData.add(time_point<steady_clock>(steady_clock::now()));
    anyData.add(FigureGeometryCalculator::getHorizontalLines(scaleDbfsToPercents(moveDbFsToPositiveValues(config.get<HorizontalLinePositions>()))));
    anyData.add(FigureGeometryCalculator::getVerticalLines(config.get<NumberOfRectangles>(), anyData.get<DataSelector>().getRectangleIndexesClosestToFrequencies(config.get<VerticalLinePositions>())));
    anyData.add(FigureGeometryCalculator::getVerticalLineTextPositions(config.get<NumberOfRectangles>(), anyData.get<DataSelector>().getRectangleIndexesClosestToFrequencies(config.get<FrequencyTextPositions>())));
    anyData.add(DynamicMaxHolders{{MaxHolderType::Dynamic,DynamicMaxHolder(config.get<NumberOfRectangles>(), config.get<DynamicMaxHoldSpeedOfFalling>(), config.get<DynamicMaxHoldAccelerationStateOfFalling>())},
                                  {MaxHolderType::Transparent,DynamicMaxHolder(config.get<NumberOfRectangles>(), config.get<DynamicMaxHoldSecondarySpeedOfFalling>(), false)}});

    anyData.add(RectangleHighligther(config.get<NumberOfRectangles>()));

    createWindow();
}

void Window::initializeGPU()
{
    gpu.init();
    gpu.enableTransparency();
    gpu.initLines();
    gpu.initText();
    gpu.initRectangles(config.get<BackgroundColorSettings>(), config.get<AdvancedColorSettings>());
    gpu.prepareBackground(FigureGeometryCalculator::rectanglesFactory(100, 1,100,0,0,100));
    gpu.prepareRectangles(FigureGeometryCalculator::rectanglesFactory(100, config.get<NumberOfRectangles>(), 0, config.get<GapWidthInRelationToRectangleWidth>()), config.get<ColorsOfRectangle>());
    gpu.prepareDynamicMaxHoldSecondaryRectangles(FigureGeometryCalculator::rectanglesFactory(100, config.get<NumberOfRectangles>(), 0, config.get<GapWidthInRelationToRectangleWidth>()), config.get<ColorsOfDynamicMaxHoldSecondaryRectangle>());
    gpu.prepareDynamicMaxHoldRectangles(FigureGeometryCalculator::rectanglesFactory(config.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>(), config.get<NumberOfRectangles>(), 50 + config.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>()/2, config.get<GapWidthInRelationToRectangleWidth>()), config.get<ColorsOfDynamicMaxHoldRectangle>());
    gpu.prepareHorizontalLines(config.get<HorizontalLinePositions>().size());
    gpu.prepareVerticalLines(config.get<VerticalLinePositions>().size());
    gpu.prepareDynamicLines(config.get<NumberOfRectangles>()-1);
    gpu.prepareDynamicMaxHoldLines(config.get<NumberOfRectangles>()-1);
    gpu.prepareDynamicMaxHoldSecondaryLines(config.get<NumberOfRectangles>()-1);
    gpu.prepareHorizontalLineStaticTexts(config.get<HorizontalLinePositions>(), config.get<ColorOfStaticText>());
    gpu.prepareVerticalLineStaticTexts(config.get<FrequencyTextPositions>(), config.get<ColorOfStaticText>());
    gpu.prepareDynamicText();

    operations.emplace_back("prepareData", [&](){

        auto dBFsValues = anyData.get<DataSelector>()(anyData.get<std::vector<float>>());

        anyData.get<DynamicMaxHolders>().at(MaxHolderType::Dynamic).calculate(dBFsValues);
        anyData.get<DynamicMaxHolders>().at(MaxHolderType::Transparent).calculate(dBFsValues);
        anyData.add(getWindowSize());
    });

    operations.emplace_back("clear", [&](){
        gpu.clear();
    });

    operations.emplace_back("updateParamsInShaders", [&](){
        auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - anyData.get<time_point<steady_clock>>()).count();
        auto themeNumber = getUpdatedThemeNumber();

        gpu.updateTime(timeInMs);

        if(themeNumber)
        {
            gpu.updateThemeNumber(*themeNumber);
        }
    });

    operations.emplace_back("background", [&](){
        gpu.drawBackground();
    });

    operations.emplace_back("horizontalLines", [&](){
        gpu.drawHorizontalLines(anyData.get<FigureGeometryCalculator::HorizontalLines>().lines, config.get<ColorOfStaticLines>());
    });

    operations.emplace_back("verticalLines", [&](){
        gpu.drawVerticalLines(anyData.get<FigureGeometryCalculator::VerticalLines>().lines, config.get<ColorOfStaticLines>());
    });

    operations.emplace_back("horizontalLineStaticTexts", [&](){

        gpu.drawHorizontalLineStaticTexts(anyData.get<FigureGeometryCalculator::HorizontalLines>().lines, anyData.get<WindowSize>(), FigureGeometryCalculator::xDrawOffsetInPercents, FigureGeometryCalculator::xDrawSizeInPercents);
    });

    operations.emplace_back("verticalLineStaticTexts", [&](){
        gpu.drawVerticalLineStaticTexts(anyData.get<FigureGeometryCalculator::VerticalLineTextPositions>().positions, anyData.get<WindowSize>());
    });

    if(config.get<RectanglesVisibilityState>() && config.get<DynamicMaxHoldSecondaryVisibilityState>())
    {
        operations.emplace_back("transparentDynamicMaxHold", [&](){
            const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DynamicMaxHolders>().at(MaxHolderType::Transparent).get()));
            gpu.drawDynamicMaxHoldSecondaryRectangles(dynamicMaxHoldElementsPosition);
        });
    }

    if(config.get<RectanglesVisibilityState>())
    {
        operations.emplace_back("rectangles", [&](){
            auto positions = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DataSelector>()()));
            gpu.drawRectangles(positions);
        });
    }

    if(config.get<LinesVisibilityState>())
    {
        operations.emplace_back("dynamicLines", [&](){
            auto positions = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DataSelector>()()));
            gpu.drawDynamicLines(FigureGeometryCalculator::getDynamicLines(positions), config.get<ColorOfLine>());
        });
    }

    if(config.get<LinesVisibilityState>() && config.get<DynamicMaxHoldVisibilityState>())
    {
        operations.emplace_back("dynamicMaxHoldLines", [&](){
            const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DynamicMaxHolders>().at(MaxHolderType::Dynamic).get()));
            gpu.drawDynamicMaxHoldLines(FigureGeometryCalculator::getDynamicLines(dynamicMaxHoldElementsPosition), config.get<ColorOfDynamicMaxHoldLine>());
        });
    }

    if(config.get<LinesVisibilityState>() && config.get<DynamicMaxHoldSecondaryVisibilityState>())
    {
        operations.emplace_back("dynamicMaxHoldTransparentLines", [&](){
            const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DynamicMaxHolders>().at(MaxHolderType::Transparent).get()));
            gpu.drawDynamicMaxHoldSecondaryLines(FigureGeometryCalculator::getDynamicLines(dynamicMaxHoldElementsPosition), config.get<ColorOfDynamicMaxHoldSecondaryLine>());
        });
    }

    if(config.get<RectanglesVisibilityState>() && config.get<DynamicMaxHoldVisibilityState>())
    {
        operations.emplace_back("dynamicMaxHold", [&](){
            const auto dynamicMaxHoldElementsPosition = scaleDbfsToPercents(moveDbFsToPositiveValues(anyData.get<DynamicMaxHolders>().at(MaxHolderType::Dynamic).get()));
            gpu.drawDynamicMaxHoldRectangles(dynamicMaxHoldElementsPosition);

        });
    }


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
                const auto dBFsHighlightedValues = rectangleHighligther.getStringToBePrinted(config.get<Freqs>().at(hightlightData.current.index),
                                                                                             averagedDBfs.value() ,
                                                                                             anyData.get<DynamicMaxHolders>().at(MaxHolderType::Transparent).get().at(hightlightData.current.index),
                                                                                             anyData.get<DynamicMaxHolders>().at(MaxHolderType::Dynamic).get().at(hightlightData.current.index));
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
