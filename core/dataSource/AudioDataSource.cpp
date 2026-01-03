/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSource.hpp"
#include "../Helpers.hpp"
#include <iostream>


AudioDataSource::AudioDataSource()
{
    initFunctions.emplace_back("initialize", [](){
        return Pa_Initialize();
    });

    initFunctions.emplace_back("updateAudioInputDeviceParams", [&](){

        device = Pa_GetDefaultInputDevice();

        if(device == paNoDevice)
        {
            return paInvalidDevice;
        }

        inputParams.device = device;
        inputParams.channelCount = numberOfChannels;
        inputParams.sampleFormat = paInt16;
        inputParams.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;
        inputParams.hostApiSpecificStreamInfo = nullptr;

        return paNoError;
    });

    initFunctions.emplace_back("openStream", [&](){
        return Pa_OpenStream(&stream, &inputParams, nullptr, samplingRate, dataLength, paClipOff, nullptr, nullptr);
    });

    initFunctions.emplace_back("startStream", [&](){
        return Pa_StartStream(stream);
    });
}

bool AudioDataSource::initialize(uint32_t numberOfSamples, uint32_t sampleRate)
{
    dataLength = numberOfSamples;
    samplingRate = sampleRate;
    errorOccured = false;

    buffer.emplace(std::vector<int16_t>(dataLength * numberOfChannels));

    for(auto &[procedureName, procedure]: initFunctions)
    {
        const auto result = procedure();

        if(result != paNoError)
        {
            std::cout << "PortAudio error while: " <<procedureName<<" : "<<Pa_GetErrorText(result) << std::endl;
            errorOccured = true;
            closeStreamAndBuffer();
            return false;
        }
    }

    return true;
}

bool AudioDataSource::checkIfErrorOccured()
{
    return errorOccured;
}

void AudioDataSource::updateBuffer()
{
    PaError err = Pa_ReadStream(stream, buffer.value().data(), dataLength);

    if ((err != paNoError) && (err != paInputOverflowed))
    {
        std::cout << "PortAudio error while collecting samples from hw: " << Pa_GetErrorText(err) << std::endl;
        buffer = std::nullopt;
        checkIfCriticalErrorOccured(err);
    }
}

std::vector<float> AudioDataSource::collectDataFromHw()
{
    updateBuffer();

    if(buffer != std::nullopt)
    {
        std::vector<float> leftChannel;
        std::vector<float> rightChannel;

        rightChannel.reserve(dataLength);
        leftChannel.reserve(dataLength);

        for (uint32_t i = 0; i < buffer.value().size(); ++i)
        {
            (i % 2) ? rightChannel.emplace_back(buffer.value()[i]) : leftChannel.emplace_back(buffer.value()[i]);
        }
        return getAverage(leftChannel, rightChannel);
    }
    return {};
}

void AudioDataSource::checkIfCriticalErrorOccured(const PaError &err)
{
    const PaError criticalErrors[] = {
        paNotInitialized,
        paUnanticipatedHostError,
        paInternalError,
        paDeviceUnavailable,
        paHostApiNotFound,
        paInvalidHostApi
    };

    if (std::any_of(std::begin(criticalErrors), std::end(criticalErrors),
                    [err](const PaError &e){ return e == err; }))
    {
        errorOccured = true;
        closeStreamAndBuffer();
    }
}

void AudioDataSource::closeStreamAndBuffer()
{
    if (stream)
    {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        Pa_Terminate();
        stream = nullptr;
        buffer = std::nullopt;
    }
}

AudioDataSource::~AudioDataSource()
{
    closeStreamAndBuffer();
}
