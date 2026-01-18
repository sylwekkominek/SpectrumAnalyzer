/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSourceDevice.hpp"
#include <iostream>


PaDeviceIndex getAudioSourceDevice()
{
    std::cout<<"Using microphone as the data source device"<<std::endl;
    return Pa_GetDefaultInputDevice();
}
