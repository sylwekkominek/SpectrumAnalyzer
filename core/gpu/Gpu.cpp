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
    LinesInsideGpu::initialize();
}

void Gpu::initText()
{
    TextInsideGpu::initialize();
}

void Gpu::initRectangles(const std::string &backgroundConfig, const std::string &rectanglesConfig)
{
    RectanglesInsideGpu<RectangleType::BACKGROUND>::initialize(backgroundConfig.c_str());
    RectanglesInsideGpu<RectangleType::BAR>::initialize(rectanglesConfig.c_str());
    RectanglesInsideGpu<RectangleType::SECONDARY_BAR>::initialize();
}

void Gpu::prepareBackground(const Rectangles &rectangles)
{
    background = std::make_unique<RectanglesInsideGpu<RectangleType::BACKGROUND>>(rectangles);
}

void Gpu::prepareRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    this->rectangles = std::make_unique<RectanglesInsideGpu<RectangleType::BAR>>(rectangles,  colorsOfRectangle);
}

void Gpu::prepareDynamicMaxHoldRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    dynamicMaxHoldRectangles = std::make_unique<RectanglesInsideGpu<RectangleType::BAR>>(rectangles,  colorsOfRectangle);
}

void Gpu::prepareDynamicMaxHoldSecondaryRectangles(const Rectangles &rectangles, const ColorsOfRectanglePerVertices &colorsOfRectangle)
{
    dynamicMaxHoldSecondaryRectangles = std::make_unique<RectanglesInsideGpu<RectangleType::SECONDARY_BAR>>(rectangles,  colorsOfRectangle);
}

void Gpu::prepareHorizontalLines(const uint16_t size, const Color &colorOfStaticLines)
{
    horizontalLines = std::make_unique<LinesInsideGpu>(size, colorOfStaticLines);
}

void Gpu::prepareVerticalLines(const uint16_t size, const Color &colorOfStaticLines)
{
    verticalLines = std::make_unique<LinesInsideGpu>(size, colorOfStaticLines);
}

void Gpu::prepareDynamicLines(const uint16_t size, const Color &colorOfDynamicLines)
{
    dynamicLines = std::make_unique<LinesInsideGpu>(size, colorOfDynamicLines);
}

void Gpu::prepareDynamicMaxHoldLines(const uint16_t size, const Color &colorOfDynamicLines)
{
    dynamicMaxHoldLines = std::make_unique<LinesInsideGpu>(size, colorOfDynamicLines);
}

void Gpu::prepareDynamicMaxHoldSecondaryLines(const uint16_t size, const Color &colorOfDynamicLines)
{
    dynamicMaxHoldSecondaryLines = std::make_unique<LinesInsideGpu>(size, colorOfDynamicLines);
}

void Gpu::prepareHorizontalLineStaticTexts(const std::vector<float> &dbfsValues, const Color &colorOfStaticLines)
{
    for(uint16_t i=0;i<dbfsValues.size();++i)
    {
        horizontalLineStaticTexts.emplace_back(formatFloat(dbfsValues[i],4,2), colorOfStaticLines);
    }
}

void Gpu::prepareVerticalLineStaticTexts(const Frequencies &frequencies, const Color &colorOfStaticLines)
{
    for(uint16_t i=0;i<frequencies.size();++i)
    {
        verticalLineStaticTexts.emplace_back(formatFloat(frequencies[i],4,0), colorOfStaticLines);
    }
}

void Gpu::prepareHighlightedVerticalLine(const Color &color)
{
    highlightedVerticalLine = std::make_unique<LinesInsideGpu>(1, color);
}

void Gpu::prepareDynamicText()
{
    dynamicText = std::make_unique<TextInsideGpu>("", std::vector<float>{1.0,0,0,1});
}

void Gpu::updateTime(const float timeInMilliSeconds)
{
    RectanglesInsideGpu<RectangleType::BAR>::updateTime(timeInMilliSeconds);
    RectanglesInsideGpu<RectangleType::SECONDARY_BAR>::updateTime(timeInMilliSeconds);
    RectanglesInsideGpu<RectangleType::BACKGROUND>::updateTime(timeInMilliSeconds);
}

void Gpu::drawBackground()
{
    background->draw();
}

void Gpu::drawHorizontalLines(const Lines &horizontalLinePositions)
{
    if(!horizontalLinePositions.empty())
    {
        horizontalLines->draw(horizontalLinePositions);
    }
}

void Gpu::drawVerticalLines(const Lines &verticalLinePositions)
{
    if(!verticalLinePositions.empty())
    {
        verticalLines->draw(verticalLinePositions);
    }
}

void Gpu::drawDynamicLines(const Lines &dynamicLinePositions)
{
    if(!dynamicLinePositions.empty())
    {
        dynamicLines->draw(dynamicLinePositions);
    }
}

void Gpu::drawDynamicMaxHoldLines(const Lines &dynamicLinePositions)
{
    if(!dynamicLinePositions.empty())
    {
        dynamicMaxHoldLines->draw(dynamicLinePositions);
    }
}

void Gpu::drawDynamicMaxHoldSecondaryLines(const Lines &dynamicLinePositions)
{
    if(!dynamicLinePositions.empty())
    {
        dynamicMaxHoldSecondaryLines->draw(dynamicLinePositions);
    }
}

void Gpu::drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize, const float xDrawOffsetInPercents, const float xDrawSizeInPercents)
{
    const float fullScreenSizeInPercents{100};
    const float leftCenterOfTheOffset= (xDrawOffsetInPercents / 100)/2;
    const float offsetRigth = fullScreenSizeInPercents - (xDrawSizeInPercents + xDrawOffsetInPercents);
    const float rightCenterOfTheOffset = ((fullScreenSizeInPercents - offsetRigth/2)/100);
    const float workaroundForNotPerfectlyAlignedFont = 0.001;

    const uint32_t numberOfElements = std::min(horizontalLineStaticTexts.size(), horizontalLinePositions.size());

    for(uint32_t i=0;i<numberOfElements;++i)
    {
        const auto textPositionInPixels = convertYPositionInPercentToPixels(horizontalLinePositions[i].front().y, windowSize.y);

        horizontalLineStaticTexts[i].draw(HorizontalAligment::CENTER, VerticalAligment::CENTER, (leftCenterOfTheOffset - workaroundForNotPerfectlyAlignedFont) * windowSize.x ,textPositionInPixels);
        horizontalLineStaticTexts[i].draw(HorizontalAligment::CENTER, VerticalAligment::CENTER, (rightCenterOfTheOffset + workaroundForNotPerfectlyAlignedFont) * windowSize.x , textPositionInPixels);
    }
}

void Gpu::drawHorizontalLineStaticTexts(const Lines &horizontalLinePositions, const WindowSize &windowSize)
{
    const float workaroundForNotPerfectlyAlignedFont = 0.001;

    const uint32_t numberOfElements = std::min(horizontalLineStaticTexts.size(), horizontalLinePositions.size());


    for(uint32_t i=0;i<numberOfElements;++i)
    {
        const auto textPositionInPixels = convertYPositionInPercentToPixels(horizontalLinePositions[i].front().y, windowSize.y);

        horizontalLineStaticTexts[i].draw(HorizontalAligment::CENTER, VerticalAligment::CENTER, (0.5 + workaroundForNotPerfectlyAlignedFont) * windowSize.x ,textPositionInPixels);
    }
}

void Gpu::drawVerticalLineStaticTexts(const Positions &verticalLineTextPositions, const WindowSize &windowSize)
{
    const float offsetFromTheTop = 0.025;

    const uint32_t numberOfElements = std::min(verticalLineStaticTexts.size(), verticalLineTextPositions.size());

    for(uint32_t i=0 ; i< numberOfElements; ++i)
    {
        const auto textPositionInPixels = convertXPositionInPercentToPixels(verticalLineTextPositions[i], windowSize.x);
        verticalLineStaticTexts[i].draw(HorizontalAligment::CENTER, VerticalAligment::TOP, textPositionInPixels, offsetFromTheTop * windowSize.y);
    }
}

void Gpu::drawDynamicMaxHoldRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition)
{
    if(!dynamicMaxHoldElementsPosition.empty())
    {
        dynamicMaxHoldRectangles->move(dynamicMaxHoldElementsPosition);
    }
}

void Gpu::drawDynamicMaxHoldSecondaryRectangles(const std::vector<float> &dynamicMaxHoldElementsPosition)
{
    if(!dynamicMaxHoldElementsPosition.empty())
    {
        dynamicMaxHoldSecondaryRectangles->move(dynamicMaxHoldElementsPosition);
    }
}

void Gpu::drawRectangles(const std::vector<float> &rectaglesPositions)
{
    if(!rectaglesPositions.empty())
    {
        rectangles->move(rectaglesPositions);
    }
}

void Gpu::updateHorizontalRectangleBoundaries(const uint16_t indexOfRectangle, const float start, const float stop)
{
    rectangles->updateBoundary(start, stop);
}

void Gpu::drawHighlightedVerticalLine(const Line &line)
{
    highlightedVerticalLine->draw({line});
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
    RectanglesInsideGpu<RectangleType::BACKGROUND>::finalize();
    RectanglesInsideGpu<RectangleType::SECONDARY_BAR>::finalize();
    RectanglesInsideGpu<RectangleType::BAR>::finalize();
    LinesInsideGpu::finalize();
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

