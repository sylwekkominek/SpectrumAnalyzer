/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>

struct BackgroundColorSettings
{
    BackgroundColorSettings(const std::string &value);
    BackgroundColorSettings(const ThemeConfig themeConfig);

    std::string getBackgroundColorSettings();
    std::string getInfo();
    std::string value;
    const std::string name{"BackgroundColorSettings"};

private:
    std::string getBackgroundColorSettings(const ThemeConfig themeConfig);
};
