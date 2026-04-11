/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <ostream>

struct SignalWindow
{
    SignalWindow(const Mode mode, uint16_t numberOfSamples);
    SignalWindow(const std::vector<float> &window);
    std::vector<float> getSignalWindow(uint16_t numberOfSamples);
    std::string getInfo();
    Mode mode;
    std::vector<float> value;
    const std::string name{"SignalWindow"};
};

std::ostream& operator<<(std::ostream& os, const SignalWindow& signalWindow);
