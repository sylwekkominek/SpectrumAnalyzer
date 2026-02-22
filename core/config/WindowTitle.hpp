/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>

struct WindowTitle
{
    WindowTitle(const std::string &value);
    WindowTitle(const ThemeConfig themeConfig);
    std::string getAdvancedColorSettings();
    std::string getInfo();
    std::string value;
    const std::string name{"WindowTitle"};
private:
    std::string getWindowTitle(const ThemeConfig themeConfig);
};

