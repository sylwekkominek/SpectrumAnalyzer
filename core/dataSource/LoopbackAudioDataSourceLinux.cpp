/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSourceDevice.hpp"
#include <iostream>

PaDeviceIndex getAudioSourceDevice()
{
    std::cout<<"Using loopback as the data source device"<<std::endl;

    PaDeviceIndex device{paNoDevice};

    const int16_t numDevices = Pa_GetDeviceCount();

    if (numDevices < 0)
    {
        return device;
    }

    for (PaDeviceIndex i = 0; i < numDevices; ++i)
    {
        const PaDeviceInfo* info = Pa_GetDeviceInfo(i);

        if (!info || info->maxInputChannels <= 0)
        {
            continue;
        }

        const std::string name = info->name;

        if (name.find(".monitor") != std::string::npos)
        {
            std::cout << "Selected first monitor device: " << name << " (index " << i << ")" << std::endl;
            return i;
        }
    }

    return device;
}
