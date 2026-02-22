/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct DefaultFullscreenState
{
    DefaultFullscreenState(bool value);
    DefaultFullscreenState(const ThemeConfig themeConfig);
    std::string getInfo();
    bool value;
    const std::string name{"DefaultFullscreenState"};
private:
    bool getDefaultFullscreenState(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const DefaultFullscreenState& defaultFullscreenState);

