/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct DynamicMaxHoldSecondaryVisibilityState
{
    DynamicMaxHoldSecondaryVisibilityState(bool value);
    DynamicMaxHoldSecondaryVisibilityState(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    bool value;
    const std::string name{"DynamicMaxHoldSecondaryVisibilityState"};
private:
    template <Mode>
    bool getDynamicMaxHoldSecondaryVisibilityState(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSecondaryVisibilityState &dynamicMaxHoldSecondaryVisibilityState);

