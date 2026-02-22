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

struct NormalWindowSize
{
    NormalWindowSize(std::pair<uint16_t, uint16_t> value);
    NormalWindowSize(const ThemeConfig themeConfig);
    std::string getInfo();
    std::pair<uint16_t, uint16_t> value;
    const std::string name{"NormalWindowSize"};
private:
    std::pair<uint16_t, uint16_t> getNormalWindowSize(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const NormalWindowSize &normalWindowSize);
