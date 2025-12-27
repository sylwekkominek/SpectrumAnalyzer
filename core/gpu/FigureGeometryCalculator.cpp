/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FigureGeometryCalculator.hpp"

std::vector<std::pair<float, float>> FigureGeometryCalculator::horizontalRectanglesBoundariesFactory(const uint16_t numberOfRectangles, const float gap)
{
    const double fullScreenSize = 2.0;
    const double xBeginOfZeroElement = -1;
    const double numberOfGaps = numberOfRectangles -1;
    const double xWidth =  (fullScreenSize/(numberOfRectangles + numberOfGaps * gap));

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

std::vector<Rectangle> FigureGeometryCalculator::rectanglesFactory(const float heightInPercentOfScreenSize, const uint16_t numberOfRectangles, const float offsetInPercentOffScreenSize, const float gap)
{
    const auto xBoundaries = horizontalRectanglesBoundariesFactory(numberOfRectangles, gap);

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

std::vector<Line> FigureGeometryCalculator::getHorizontalLines(const Positions &positions)
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

