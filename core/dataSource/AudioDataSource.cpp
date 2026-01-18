/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "AudioDataSource.hpp"
#include "AudioDataSourceDevice.hpp"
#include "../Helpers.hpp"
#include <iostream>
#include <chrono>
#include <thread>

AudioDataSource::AudioDataSource()
{
    initFunctions.emplace_back("initialize", [](){
        return Pa_Initialize();
    });

    initFunctions.emplace_back("updateAudioInputDevice", [&](){

        device = getAudioSourceDevice();

        if(device == paNoDevice)
        {
            return paInvalidDevice;
        }

        return paNoError;
    });

    initFunctions.emplace_back("updateAudioInputDeviceParams", [&](){

        inputParams.device = device;
        inputParams.channelCount = numberOfChannels;
        inputParams.sampleFormat = paInt16;
        inputParams.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;
        inputParams.hostApiSpecificStreamInfo = nullptr;

        PaError err = Pa_IsFormatSupported(
            &inputParams,
            nullptr,
            samplingRate
            );

        if(err != paNoError)
        {
            auto deviceInfo = Pa_GetDeviceInfo(device);
            const auto supportedSampleRate = deviceInfo->defaultSampleRate;
            const char* deviceName = deviceInfo->name;

            std::ostringstream warning;

            warning << R"(WARNING: THE APPLICATION IS USING THE DEFAULT AUDIO CONFIGURATION
FROM WINDOWS BECAUSE THE SAMPLE RATE SPECIFIED IN THE CONFIG FILE IS NOT
SUPPORTED BY THE CURRENT OUTPUT DEVICE: )" << deviceName << R"(
AUDIO FREQUENCIES MAY BE CALCULATED INCORRECTLY.

FIX OPTION 1: MODIFY SamplingRate.txt FILE WITH SAMPLE RATE: )" << supportedSampleRate << R"(

FIX OPTION 2: CHANGE THE DEFAULT AUDIO FORMAT IN WINDOWS:
            CLICK THE SPEAKER ICON -> SOUND SETTINGS -> DEVICE PROPERTIES ->
            OUTPUT SETTINGS -> FORMAT -> CHOOSE SAMPLE RATE: )" << samplingRate;

            std::cout << std::endl<<warning.str() << std::endl;

            samplingRate = supportedSampleRate;
         }

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
    if (isDataAvailable())
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
    }

    std::cout << "No input data " <<std::endl;
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

bool AudioDataSource::isDataAvailable()
{
    constexpr int maxWaitMs = 60;
    constexpr int delayInMs = 1;

    int waitedMs = 0;

    while (waitedMs < maxWaitMs)
    {
        if (Pa_GetStreamReadAvailable(stream) >= dataLength)
        {
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delayInMs));
        waitedMs += delayInMs;
    }

    return false;
}


AudioDataSource::~AudioDataSource()
{
    closeStreamAndBuffer();
}
