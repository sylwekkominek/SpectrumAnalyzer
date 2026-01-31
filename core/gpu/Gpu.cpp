/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Gpu.hpp"
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

void Gpu::initLines()
{
    LineInsideGpu::initialize();
}

void Gpu::initText()
{
    TextInsideGpu::initialize();
}

void Gpu::initRectangles(const std::string &backgroundConfig, const std::string &rectanglesConfig)
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::initialize(backgroundConfig.c_str());
    RectangleInsideGpu<RectangleType::BAR>::initialize(rectanglesConfig.c_str());
    RectangleInsideGpu<RectangleType::SECONDARY_BAR>::initialize();
}

void Gpu::prepareBackground(const Rectangles &rectangles)
{
    background = std::make_unique<RectangleInsideGpu<RectangleType::BACKGROUND>>(rectangles.front());
}

void Gpu::prepareRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    for(const auto & rectangle : rectangles)
    {
        this->rectangles.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle, colorsOfRectangle));
    }
}

void Gpu::prepareDynamicMaxHoldRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    for(const auto & rectangle : rectangles)
    {
        dynamicMaxHoldRectangles.emplace_back(RectangleInsideGpu<RectangleType::BAR>(rectangle,  colorsOfRectangle));
    }
}

void Gpu::prepareDynamicMaxHoldSecondaryRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    for(const auto & rectangle : rectangles)
    {
        dynamicMaxHoldSecondaryRectangles.emplace_back(RectangleInsideGpu<RectangleType::SECONDARY_BAR>(rectangle,  colorsOfRectangle));
    }
}

void Gpu::prepareHorizontalLines(const uint16_t size)
{
    for(uint16_t i=0;i<size;++i)
    {
        horizontalLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareVerticalLines(const uint16_t size)
{
    for(uint16_t i=0;i<size;++i)
    {
        verticalLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareDynamicLines(const uint16_t size)
{
    for(uint16_t i=0;i<size;++i)
    {
        dynamicLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareDynamicMaxHoldLines(const uint16_t size)
{
    for(uint16_t i=0;i<size;++i)
    {
        dynamicMaxHoldLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareDynamicMaxHoldSecondaryLines(const uint16_t size)
{
    for(uint16_t i=0;i<size;++i)
    {
        dynamicMaxHoldSecondaryLines.emplace_back(LineInsideGpu());
    }
}

void Gpu::prepareHorizontalLineStaticTexts(const std::vector<float> &dbfsValues, const Color &colorOfStaticLines)
{
    for(uint16_t i=0;i<dbfsValues.size();++i)
    {
        horizontalLineStaticTexts.emplace_back(formatFloat(dbfsValues.at(i),4,2), colorOfStaticLines);
    }
}

void Gpu::prepareVerticalLineStaticTexts(const Frequencies &frequencies, const Color &colorOfStaticLines)
{
    for(uint16_t i=0;i<frequencies.size();++i)
    {
        verticalLineStaticTexts.emplace_back(formatFloat(frequencies.at(i),4,0), colorOfStaticLines);
    }
}

void Gpu::prepareDynamicText()
{
    dynamicText = std::make_unique<TextInsideGpu>("", std::vector<float>{1.0,0,0,1});
}

void Gpu::updateTime(const float timeInMilliSeconds)
{
    RectangleInsideGpu<RectangleType::BAR>::updateTime(timeInMilliSeconds);
    RectangleInsideGpu<RectangleType::SECONDARY_BAR>::updateTime(timeInMilliSeconds);
    RectangleInsideGpu<RectangleType::BACKGROUND>::updateTime(timeInMilliSeconds);
}

void Gpu::updateThemeNumber(const uint16_t themeNumber)
{
    RectangleInsideGpu<RectangleType::BAR>::updateThemeNumber(themeNumber);
    RectangleInsideGpu<RectangleType::BACKGROUND>::updateThemeNumber(themeNumber);
}

void Gpu::drawBackground()
{
    background->draw();
}

void Gpu::drawHorizontalLines(const Lines &horizontalLinePositions, const Color &colorOfStaticLines)
{
    for(uint32_t i=0;i<horizontalLinePositions.size();++i)
    {
        horizontalLines.at(i).draw(horizontalLinePositions.at(i), colorOfStaticLines);
    }
}

void Gpu::drawVerticalLines(const Lines &verticalLinePositions, const Color &colorOfStaticLines)
{
    for(uint32_t i=0;i<verticalLinePositions.size();++i)
    {
        verticalLines.at(i).draw(verticalLinePositions.at(i), colorOfStaticLines);
    }
}

void Gpu::drawDynamicLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines)
{
    for(uint32_t i=0;i<dynamicLinePositions.size();++i)
    {
        dynamicLines.at(i).draw(dynamicLinePositions.at(i), colorOfDynamicLines);
    }
}

void Gpu::drawDynamicMaxHoldLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines)
{
    for(uint32_t i=0;i<dynamicLinePositions.size();++i)
    {
        dynamicMaxHoldLines.at(i).draw(dynamicLinePositions.at(i), colorOfDynamicLines);
    }
}

void Gpu::drawDynamicMaxHoldSecondaryLines(const Lines &dynamicLinePositions, const Color &colorOfDynamicLines)
{
    for(uint32_t i=0;i<dynamicLinePositions.size();++i)
    {
        dynamicMaxHoldSecondaryLines.at(i).draw(dynamicLinePositions.at(i), colorOfDynamicLines);
    }
}

void Gpu::drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize, const float xDrawOffsetInPercents, const float xDrawSizeInPercents)
{
    const float fullScreenSizeInPercents{100};
    const float leftCenterOfTheOffset= (xDrawOffsetInPercents / 100)/2;
    const float offsetRigth = fullScreenSizeInPercents - (xDrawSizeInPercents + xDrawOffsetInPercents);
    const float rightCenterOfTheOffset = ((fullScreenSizeInPercents - offsetRigth/2)/100);

    const float workaroundForNotPerfectlyAlignedFont = 0.001;

    for(uint32_t i=0;i<horizontalLinePositions.size();++i)
    {
        const auto textPositionInPixels = convertYPositionInPercentToPixels(horizontalLinePositions.at(i).front().y, windowSize.y);

        horizontalLineStaticTexts.at(i).draw(HorizontalAligment::CENTER, VerticalAligment::CENTER, (leftCenterOfTheOffset - workaroundForNotPerfectlyAlignedFont) * windowSize.x ,textPositionInPixels);
        horizontalLineStaticTexts.at(i).draw(HorizontalAligment::CENTER, VerticalAligment::CENTER, (rightCenterOfTheOffset + workaroundForNotPerfectlyAlignedFont) * windowSize.x , textPositionInPixels);
    }
}

void Gpu::drawVerticalLineStaticTexts(const Positions &verticalLineTextPositions, const WindowSize &windowSize)
{
    const float offsetFromTheTop = 0.025;

    for(uint32_t i=0;i<verticalLineTextPositions.size();++i)
    {
        const auto textPositionInPixels = convertXPositionInPercentToPixels(verticalLineTextPositions.at(i), windowSize.x);
        verticalLineStaticTexts.at(i).draw(HorizontalAligment::CENTER, VerticalAligment::TOP, textPositionInPixels, offsetFromTheTop * windowSize.y);
    }
}

void Gpu::drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition)
{
    for(uint32_t i=0;i<dynamicMaxHoldRectangles.size();++i)
    {
        dynamicMaxHoldRectangles.at(i).move(dynamicMaxHoldElementsPosition.at(i));
    }
}

void Gpu::drawDynamicMaxHoldSecondaryRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition)
{
    for(uint32_t i=0;i<dynamicMaxHoldRectangles.size();++i)
    {
        dynamicMaxHoldSecondaryRectangles.at(i).move(dynamicMaxHoldElementsPosition.at(i));
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
    dynamicText->draw(str, aligment, VerticalAligment::BOTTOM, x, y);
}

void Gpu::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

Gpu::~Gpu()
{
    RectangleInsideGpu<RectangleType::BACKGROUND>::finalize();
    RectangleInsideGpu<RectangleType::SECONDARY_BAR>::finalize();
    RectangleInsideGpu<RectangleType::BAR>::finalize();
    LineInsideGpu::finalize();
    TextInsideGpu::finalize();
}

float Gpu::convertXPositionInPercentToPixels(const float positionInPercents, const float xScreenSize)
{
    auto position = (positionInPercents)/100;
    return position * xScreenSize;
}

float Gpu::convertYPositionInPercentToPixels(const float positionInPercents, const float yScreenSize)
{
    auto position = (100-positionInPercents)/100;
    return position * yScreenSize;
}

