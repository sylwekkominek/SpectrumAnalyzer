/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct ColorsOfRectangle
{

    ColorsOfRectangle(const ColorsOfRectanglePerVertices &value);
    ColorsOfRectangle(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    ColorsOfRectanglePerVertices value;
    const std::string name{"ColorsOfRectangle"};

private:
    template <Mode>
    ColorsOfRectanglePerVertices getColorsOfRectangle(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const ColorsOfRectangle &colorsOfRectangle);

