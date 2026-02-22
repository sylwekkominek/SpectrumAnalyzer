/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct GapWidthInRelationToRectangleWidth
{
    GapWidthInRelationToRectangleWidth(float value);
    GapWidthInRelationToRectangleWidth(const ThemeConfig themeConfig);
    std::string getInfo();
    float value;
    const std::string name{"GapWidthInRelationToRectangleWidth"};
private:
    float getGapWidthInRelationToRectangleWidth(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const GapWidthInRelationToRectangleWidth& gapWidthInRelationToRectangleWidth);
