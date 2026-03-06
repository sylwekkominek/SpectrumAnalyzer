/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSourceDevice.hpp"
#include <iostream>


PaDeviceIndex getAudioInputDevice()
{
    std::cout<<"Using microphone or other input device as the audio source"<<std::endl;
    return Pa_GetDefaultInputDevice();
}
