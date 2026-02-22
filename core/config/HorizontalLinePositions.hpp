/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct HorizontalLinePositions
{
    HorizontalLinePositions(const Positions &value);
    HorizontalLinePositions(const ThemeConfig themeConfig);
    std::string getInfo();
    Positions value;
    const std::string name{"HorizontalLinePositions"};
private:
    Positions getHorizontalLinePositions(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const HorizontalLinePositions &horizontalLinePositions);
