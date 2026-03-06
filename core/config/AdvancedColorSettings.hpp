/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "../CommonTypes.hpp"
#include <string>

struct AdvancedColorSettings
{
    AdvancedColorSettings(const std::string &value);
    AdvancedColorSettings(const ThemeConfig themeConfig, const Mode mode);
    std::string getAdvancedColorSettings();
    std::string getInfo();
    std::string value;
    const std::string name{"AdvancedColorSettings"};
private:
    template <Mode>
    std::string getAdvancedColorSettings(const ThemeConfig themeConfig);
};

