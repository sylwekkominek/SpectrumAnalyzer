/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>


struct DynamicMaxHoldSecondarySpeedOfFalling
{
    DynamicMaxHoldSecondarySpeedOfFalling(float value);
    DynamicMaxHoldSecondarySpeedOfFalling(const ThemeConfig themeConfig);
    std::string getInfo();
    float value;
    const std::string name{"DynamicMaxHoldSecondarySpeedOfFalling"};
    float getDynamicMaxHoldSecondarySpeedOfFalling(const ThemeConfig themeConfig);
};

