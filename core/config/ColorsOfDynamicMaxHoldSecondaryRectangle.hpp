/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>

struct ColorsOfDynamicMaxHoldSecondaryRectangle
{
    ColorsOfDynamicMaxHoldSecondaryRectangle(const ColorsOfRectanglePerVertices &value);
    ColorsOfDynamicMaxHoldSecondaryRectangle(const ThemeConfig themeConfig);
    std::string getInfo();
    ColorsOfRectanglePerVertices value;
    const std::string name{"ColorsOfDynamicMaxHoldSecondaryRectangle"};

private:
    ColorsOfRectanglePerVertices getColorsOfDynamicMaxHoldSecondaryRectangle(const ThemeConfig themeConfig);
};



