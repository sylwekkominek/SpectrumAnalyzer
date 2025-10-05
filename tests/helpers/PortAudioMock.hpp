/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <glad/glad.h>
#include <gmock/gmock.h>
#include <portaudio.h>

class PortAudioMock
{
public:
    PortAudioMock();

    MOCK_METHOD0(Pa_Initialize, PaError());
    MOCK_METHOD1(Pa_GetErrorText, const char *(PaError errorCode));
    MOCK_METHOD0(Pa_GetDefaultInputDevice, const PaDeviceIndex());
    MOCK_METHOD1(Pa_GetDeviceInfo, const PaDeviceInfo*(PaDeviceIndex));
    MOCK_METHOD8(Pa_OpenStream, PaError(PaStream**,const PaStreamParameters *,const PaStreamParameters *,double,unsigned long,PaStreamFlags,PaStreamCallback *,void *));
    MOCK_METHOD1(Pa_StartStream, PaError(PaStream *));
    MOCK_METHOD3(Pa_ReadStream, PaError(PaStream*,void *,unsigned long));
    MOCK_METHOD1(Pa_StopStream, PaError(PaStream *));
    MOCK_METHOD1(Pa_CloseStream, PaError(PaStream *));
    MOCK_METHOD0(Pa_Terminate, PaError());
};


