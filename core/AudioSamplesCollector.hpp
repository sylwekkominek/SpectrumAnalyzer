/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "SamplesCollectorBase.hpp"
#include <vector>
#include <portaudio.h>
#include <functional>
#include <string>

class AudioSamplesCollector : public SamplesCollectorBase
{
public:
    AudioSamplesCollector();
    ~AudioSamplesCollector();
    bool initialize(uint32_t numberOfSamples, uint32_t sampleRate) override;
    std::vector<float> collectDataFromHw() override;

    AudioSamplesCollector(AudioSamplesCollector&) = delete;
    AudioSamplesCollector(AudioSamplesCollector&&) = delete;
    AudioSamplesCollector& operator=(const AudioSamplesCollector&) = delete;
    AudioSamplesCollector& operator=(AudioSamplesCollector&&) = delete;

private:
    void updateBuffer();

    std::vector<std::pair<std::string, std::function<PaError()>>> initFunctions;
    PaStreamParameters inputParams{};
    PaStream* stream{};
    PaDeviceIndex device{paNoDevice};
    uint32_t samplingRate{};
    std::vector<int16_t> buffer;
};


