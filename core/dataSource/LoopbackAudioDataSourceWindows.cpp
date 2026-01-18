/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSourceDevice.hpp"
#include "pa_win_wasapi.h"

#include <iostream>
#include <string>

PaDeviceIndex getAudioSourceDevice()
{
    std::cout<<"Using loopback as the data source device"<<std::endl;

    PaDeviceIndex device = paInvalidDevice;

    const PaDeviceIndex numDevices = Pa_GetDeviceCount();
    if (numDevices < 0)
        return device;

    const PaDeviceIndex defaultOutput = Pa_GetDefaultOutputDevice();
    if (defaultOutput == paNoDevice)
        return device;

    const PaDeviceInfo* defaultOutputInfo = Pa_GetDeviceInfo(defaultOutput);
    if (!defaultOutputInfo || !defaultOutputInfo->name)
    {
        return device;
    }

    std::cout<<"Default device: "<<defaultOutputInfo->name<<std::endl;

    for (PaDeviceIndex i = 0; i < numDevices; ++i)
    {
        if (PaWasapi_IsLoopback(i) == 1)
        {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(i);
            if (!info || !info->name)
            {
                continue;
            }

            const std::string name{info->name};

            std::cout<<"Loopback device: "<<name<<std::endl;

            if (name.find(defaultOutputInfo->name) != std::string::npos)
            {
                device = i;
                break;
            }
        }
    }

    return device;
}
