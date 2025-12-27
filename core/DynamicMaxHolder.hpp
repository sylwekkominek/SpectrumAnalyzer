/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include <vector>
#include <chrono>
#include <map>

using namespace std::chrono;

enum class MaxHolderType
{
    Dynamic,
    Transparent
};

class DynamicMaxHolder
{
public:
    DynamicMaxHolder(uint16_t numberOfMaxHoldValues, const float speedOfFalling, const bool accelerationStateOfFalling);
    void calculate(const std::vector<float> &dBFs);
    std::vector<float> get();

private:
    const float speedOfFalling;
    const bool accelerationStateOfFalling;
    std::vector<float> dynamicMaxHoldValues;
    std::vector<time_point<steady_clock>> timesWhenDynamicMaxHoldValuesHaveBeenUpdated;
};

using DynamicMaxHolders = std::map<MaxHolderType, DynamicMaxHolder>;
