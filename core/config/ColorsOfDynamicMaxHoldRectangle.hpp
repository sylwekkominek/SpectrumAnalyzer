/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct ColorsOfDynamicMaxHoldRectangle
{
    ColorsOfDynamicMaxHoldRectangle(const ColorsOfRectanglePerVertices &value);
    ColorsOfDynamicMaxHoldRectangle(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    ColorsOfRectanglePerVertices value;
    const std::string name{"ColorsOfDynamicMaxHoldRectangle"};

private:
    template <Mode>
    ColorsOfRectanglePerVertices getColorsOfDynamicMaxHoldRectangle(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const ColorsOfDynamicMaxHoldRectangle &colorsOfDynamicMaxHoldRectangle);


