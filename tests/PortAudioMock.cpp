/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "PortAudioMock.hpp"


std::function<PaError()> Pa_InitializeFunction;
std::function<const char *(PaError)> Pa_GetErrorTextFunction;
std::function<PaDeviceIndex()> Pa_GetDefaultInputDeviceFunction;
std::function<const PaDeviceInfo*(PaDeviceIndex)> Pa_GetDeviceInfoFunction;
std::function<PaError(PaStream**,const PaStreamParameters *,const PaStreamParameters *,double,unsigned long,PaStreamFlags,PaStreamCallback *,void *userData)> Pa_OpenStreamFunction;
std::function<PaError(PaStream *)> Pa_StartStreamFunction;
std::function<PaError(PaStream*,void *,unsigned long)> Pa_ReadStreamFunction;
std::function<PaError(PaStream *)> Pa_StopStreamFunction;
std::function<PaError(PaStream *)> Pa_CloseStreamFunction;
std::function<PaError()> Pa_TerminateFunction;


PaError Pa_Initialize()
{
    return Pa_InitializeFunction();
}

const char * Pa_GetErrorText(PaError errorCode)
{
    return Pa_GetErrorTextFunction(errorCode);
}

PaDeviceIndex Pa_GetDefaultInputDevice()
{
    return Pa_GetDefaultInputDeviceFunction();
}

const PaDeviceInfo* Pa_GetDeviceInfo(PaDeviceIndex device)
{
    return Pa_GetDeviceInfoFunction(device);
}

PaError Pa_OpenStream(PaStream** stream,const PaStreamParameters *inputParameters,const PaStreamParameters *outputParameters,double sampleRate,unsigned long framesPerBuffer,PaStreamFlags streamFlags,PaStreamCallback *streamCallback,void *userData )
{
    return Pa_OpenStreamFunction(stream, inputParameters, outputParameters,sampleRate, framesPerBuffer, streamFlags, streamCallback, userData);
}

PaError Pa_StartStream(PaStream *stream)
{
    return Pa_StartStreamFunction(stream);
}

PaError Pa_ReadStream(PaStream* stream,void *buffer,unsigned long frames)
{
    return Pa_ReadStreamFunction(stream, buffer, frames);
}

PaError Pa_StopStream(PaStream *stream)
{
    return Pa_StopStreamFunction(stream);
}

PaError Pa_CloseStream(PaStream *stream)
{
    return Pa_CloseStreamFunction(stream);
}

PaError Pa_Terminate()
{
    return Pa_TerminateFunction();
}

PortAudioMock::PortAudioMock()
{
    Pa_InitializeFunction = [this]()
    {
        return this->Pa_Initialize();
    };

    Pa_GetErrorTextFunction = [this](PaError errorCode)
    {
        return this->Pa_GetErrorText(errorCode);
    };

    Pa_GetDefaultInputDeviceFunction = [this]()
    {
        return this->Pa_GetDefaultInputDevice();
    };

    Pa_GetDeviceInfoFunction = [this](PaDeviceIndex device)
    {
        return this->Pa_GetDeviceInfo(device);
    };

    Pa_OpenStreamFunction = [this](PaStream** stream,const PaStreamParameters *inputParameters,const PaStreamParameters *outputParameters,double sampleRate,unsigned long framesPerBuffer,PaStreamFlags streamFlags,PaStreamCallback *streamCallback,void *userData )
    {
        return Pa_OpenStream(stream, inputParameters, outputParameters,sampleRate, framesPerBuffer, streamFlags, streamCallback, userData);
    };

    Pa_StartStreamFunction = [this](PaStream *stream)
    {
        return this->Pa_StartStream(stream);
    };

    Pa_ReadStreamFunction = [this](PaStream* stream,void *buffer,unsigned long frames)
    {
        return this->Pa_ReadStream(stream, buffer, frames);
    };

    Pa_StopStreamFunction = [this](PaStream *stream)
    {
        return this->Pa_StopStream(stream);
    };

    Pa_CloseStreamFunction = [this](PaStream *stream)
    {
        return this->Pa_CloseStream(stream);
    };

    Pa_TerminateFunction = [this]()
    {
        return this->Pa_Terminate();
    };

}


