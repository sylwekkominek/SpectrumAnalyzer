/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FigureGeometryCalculator.hpp"


// X Axis:
//
// |-- 5% gap --|======= 90% screen =======|-- 5% gap --|
// -1.0        -0.9                       0.9          1.0

float FigureGeometryCalculator::xDrawOffsetInPercents = 5;
float FigureGeometryCalculator::xDrawSizeInPercents = 90;

float FigureGeometryCalculator::getOffsetFromOffsetInPercents(const float offsetInPercents)
{
    const float xBegin= -1;
    const float xEnd = 1;
    const float fullScreenPercents = 100;

    return ((xEnd - xBegin) * offsetInPercents /fullScreenPercents) -1;
}

float FigureGeometryCalculator::getWidthFromWidthInPercents(const float widthInPercents)
{
    const float xBegin= -1;
    const float xEnd = 1;
    const float fullScreenPercents = 100;

    return (xEnd - xBegin) * widthInPercents /fullScreenPercents;
}

std::vector<std::pair<float, float>> FigureGeometryCalculator::horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles, const float gap, const float xDrawOffsetInPercents, const float xDrawSizeInPercents)
{
    const double xBeginOfZeroElement = getOffsetFromOffsetInPercents(xDrawOffsetInPercents);

    const double numberOfGaps = numberOfRectangles ;
    const double xWidth =  (getWidthFromWidthInPercents(xDrawSizeInPercents)/(numberOfRectangles + numberOfGaps * gap));

    std::vector<std::pair<float, float>> rectangles;
    rectangles.reserve(numberOfRectangles);

    for(uint32_t i=0;i<numberOfRectangles;++i)
    {
        double xBegin =  xBeginOfZeroElement + i*xWidth*(1.0 + gap);
        double xEnd = xBegin + xWidth;
        rectangles.emplace_back(std::make_pair(xBegin, xEnd));
    }

    return rectangles;
}

Rectangles FigureGeometryCalculator::rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize, const float gap, const float xStart, const float fullScreenSize)
{
    const auto xBoundaries = horizontalRectanglesBoundariesFactory(numberOfRectangles, gap,xStart, fullScreenSize);

    const float offset  = (offsetInPercentOffScreenSize/50);
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

FigureGeometryCalculator::HorizontalLines FigureGeometryCalculator::getHorizontalLines(const Positions &positions)
{
    double xBegin = 0;
    double xEnd = 100;

    Lines lines{};
    lines.reserve(positions.size());

    for(const auto & position : positions)
    {
        Line line;

        line.push_back(Point{static_cast<float>(xBegin), static_cast<float>(position)});
        line.push_back(Point{static_cast<float>(xEnd),static_cast<float>(position)});
        lines.emplace_back(std::move(line));
    }

    return HorizontalLines{std::move(lines)};
}

FigureGeometryCalculator::VerticalLines FigureGeometryCalculator::getVerticalLines(const uint16_t numberOfRectangles, const std::vector<uint16_t> &indexes)
{
    const double fullScreenSize = xDrawSizeInPercents;
    const double xWidth =  (fullScreenSize/numberOfRectangles);

    const double xBeginOfLineZero = xDrawOffsetInPercents+xWidth/2;

    Lines lines{};

    lines.reserve(indexes.size());

    for(auto & index : indexes)
    {
        double x =  xBeginOfLineZero + index*xWidth;

        Line line;
        line.push_back(Point{static_cast<float>(x), static_cast<float>(0)});
        line.push_back(Point{static_cast<float>(x),static_cast<float>(100)});
        lines.emplace_back(std::move(line));
    }

    return VerticalLines{std::move(lines)};
}

FigureGeometryCalculator::VerticalLineTextPositions FigureGeometryCalculator::getVerticalLineTextPositions(const uint16_t numberOfRectangles, const std::vector<uint16_t> &indexes)
{
    const double fullScreenSize = xDrawSizeInPercents;
    const double xWidth =  (fullScreenSize/numberOfRectangles);

    const double xBeginOfLineZero = xDrawOffsetInPercents+xWidth/2;

    Positions positions{};

    positions.reserve(indexes.size());

    for(auto & index : indexes)
    {
        positions.emplace_back((float)xBeginOfLineZero + index*xWidth);
    }

    return VerticalLineTextPositions{std::move(positions)};
}

Lines FigureGeometryCalculator::getDynamicLines(const std::vector<float> &dataToBePrinted)
{
    const uint16_t numberOfLines(dataToBePrinted.size()-1);
    const double fullScreenSize = xDrawSizeInPercents;
    const double xWidth =  (fullScreenSize/(numberOfLines+1));

    const double xBeginOfLineZero = xDrawOffsetInPercents+xWidth/2;

    std::vector<Line> lines{};

    lines.reserve(numberOfLines);

    for(uint i=0;i<numberOfLines;++i)
    {
        double xBegin =  xBeginOfLineZero + i*xWidth;
        double xEnd = xBegin + xWidth;

        Line line;
        line.push_back(Point{static_cast<float>(xBegin), static_cast<float>(dataToBePrinted.at(i))});
        line.push_back(Point{static_cast<float>(xEnd),static_cast<float>(dataToBePrinted.at(i+1))});
        lines.emplace_back(std::move(line));
    }

    return lines;
}
