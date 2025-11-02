/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Gpu.hpp"
#include "FigureGeometryCalculator.hpp"
#include "Helpers.hpp"

void Gpu::init()
{
    gladLoadGL();
}

void Gpu::enableTransparency()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Gpu::prepareBackground(const std::string &backgroundConfig)
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::initialize(backgroundConfig.c_str());
    background = std::make_unique<RectangleInsideGpu<RectangleType::BACKGROUND>>(FigureGeometryCalculator::rectanglesFactory(100, 1,100).front());
}

void Gpu::prepareRectangles(const uint16_t numberOfRectangles, const float gap, const ColorsOfRectanglePerVertices &colorsOfRectangle, const std::string &rectanglesConfig)
{
    RectangleInsideGpu<RectangleType::BAR>::initialize(rectanglesConfig.c_str());

    for(const auto & rectangle : FigureGeometryCalculator::rectanglesFactory(100, numberOfRectangles, 0, gap))
    {
        rectangles.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle, colorsOfRectangle));
    }
}

void Gpu::prepareDynamicMaxHoldRectangles(const uint16_t numberOfRectangles, const float height, const float gap, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    for(const auto & rectangle : FigureGeometryCalculator::rectanglesFactory(height, numberOfRectangles, 50 + height/2, gap))
    {
        dynamicMaxHoldRectangles.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle,  colorsOfRectangle));
    }
}

void Gpu::prepareHorizontalLines(const uint16_t size)
{
    LineInsideGpu::initialize();

    for(uint16_t i=0;i<size;++i)
    {
        horizontalLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareStaticTexts(const Positions &horizontalLinePositions, const Color &colorOfStaticLines)
{
    TextInsideGpu::initialize();

    for(uint16_t i=0;i<horizontalLinePositions.size();++i)
    {
        staticTexts.emplace_back(formatFloat(horizontalLinePositions.at(i),4,2), colorOfStaticLines);
    }
}

void Gpu::prepareDynamicText()
{
    dynamicText = std::make_unique<TextInsideGpu>("", std::vector<float>{1.0,0,0,1});
}

void Gpu::updateTime(const float timeInMilliSeconds)
{
    RectangleInsideGpu<RectangleType::BAR>::updateTime(timeInMilliSeconds);
    RectangleInsideGpu<RectangleType::BACKGROUND>::updateTime(timeInMilliSeconds);
}

void Gpu::drawBackground()
{
    background->draw();
}

void Gpu::drawHorizontalLines(const std::vector<Line> &horizontalLinePositions, const Color &colorOfStaticLines)
{
    for(uint32_t i=0;i<horizontalLinePositions.size();++i)
    {
        horizontalLines.at(i).draw(horizontalLinePositions.at(i), colorOfStaticLines);
    }
}

void Gpu::drawStaticTexts(const std::vector<Line> &horizontalLinePositions, const WindowSize &windowSize)
{
    for(uint32_t i=0;i<horizontalLinePositions.size();++i)
    {
        const auto textPositionInPixels = convertPositionInPercentToPixels(horizontalLinePositions.at(i).front().y, windowSize.y);
        staticTexts.at(i).draw(HorizontalAligment::LEFT, 0,textPositionInPixels);
        staticTexts.at(i).draw(HorizontalAligment::RIGHT, windowSize.x, textPositionInPixels);
    }
}

void Gpu::drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition)
{
    for(uint32_t i=0;i<dynamicMaxHoldRectangles.size();++i)
    {
        dynamicMaxHoldRectangles.at(i).move(dynamicMaxHoldElementsPosition.at(i));
    }
}

void Gpu::drawRectangles(const std::vector<float> &rectaglesPositions)
{
    for(uint32_t i=0;i<rectaglesPositions.size();++i)
    {
        rectangles.at(i).move(rectaglesPositions.at(i));
    }
}

void Gpu::updateHorizontalRectangleBoundaries(const uint16_t indexOfRectangle, const float start, const float stop)
{
    rectangles.at(indexOfRectangle).updateBoundary(start, stop);
}

void Gpu::drawText(const std::string &str, const HorizontalAligment aligment, const float x, const float y)
{
    dynamicText->draw(str, aligment, x, y);
}

void Gpu::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

Gpu::~Gpu()
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::finalize();
    RectangleInsideGpu<RectangleType::BAR>::finalize();
    LineInsideGpu::finalize();
    TextInsideGpu::finalize();
}
float Gpu::convertPositionInPercentToPixels(const float positionInPercents, const float screenSize)
{
    auto position = (100-positionInPercents)/100;
    return position * screenSize;
}
