/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "DynamicMaxHolder.hpp"
#include "CommonData.hpp"

DynamicMaxHolder::DynamicMaxHolder(uint16_t numberOfMaxHoldValues, const float speedOfFalling, const bool accelerationStateOfFalling):
    speedOfFalling(speedOfFalling),
    accelerationStateOfFalling(accelerationStateOfFalling),
    dynamicMaxHoldValues(numberOfMaxHoldValues, getFloorDbFs16bit()),
    timesWhenDynamicMaxHoldValuesHaveBeenUpdated(numberOfMaxHoldValues, steady_clock::now())
{
}

void DynamicMaxHolder::calculate(const std::vector<float> &dBFs)
{
    auto time = steady_clock::now();
    for(uint32_t i=0;i<dynamicMaxHoldValues.size();++i)
    {
        uint32_t diffInTime = (duration_cast<milliseconds>(time - timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i])).count();

        float diffInValue = diffInTime/speedOfFalling;
        float newValue = (dynamicMaxHoldValues[i] - diffInValue);

        newValue = newValue > getFloorDbFs16bit() ?  newValue : getFloorDbFs16bit();

        if(dBFs[i] > newValue)
        {
            dynamicMaxHoldValues[i] = dBFs[i];
            timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i] = steady_clock::now();
        }
        else
        {
            dynamicMaxHoldValues[i] =  newValue;
            timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i] = accelerationStateOfFalling ?  timesWhenDynamicMaxHoldValuesHaveBeenUpdated[i]: time;
        }
    }
}

std::vector<float> DynamicMaxHolder::get()
{
    return dynamicMaxHoldValues;
}

