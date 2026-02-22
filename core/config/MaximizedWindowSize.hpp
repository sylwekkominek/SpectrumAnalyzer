/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <cstdint>
#include <ostream>

struct MaximizedWindowSize
{
    MaximizedWindowSize(std::pair<uint16_t, uint16_t> value);
    MaximizedWindowSize(const ThemeConfig themeConfig);
    std::string getInfo();
    std::pair<uint16_t, uint16_t> value;
    const std::string name{"MaximizedWindowSize"};
private:
    std::pair<uint16_t, uint16_t> getMaximizedWindowSize(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const MaximizedWindowSize &maximizedWindowSize);

