/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "DataSourceBase.hpp"
#include <vector>
#include <portaudio.h>
#include <functional>
#include <string>
#include <optional>

class AudioDataSource : public DataSourceBase
{
public:
    AudioDataSource();
    ~AudioDataSource();
    bool initialize(uint32_t numberOfSamples, uint32_t sampleRate) override;
    bool checkIfErrorOccured() override;
    std::vector<float> collectDataFromHw() override;

    AudioDataSource(AudioDataSource&) = delete;
    AudioDataSource(AudioDataSource&&) = delete;
    AudioDataSource& operator=(const AudioDataSource&) = delete;
    AudioDataSource& operator=(AudioDataSource&&) = delete;

private:
    void updateBuffer();
    void checkIfCriticalErrorOccured(const PaError &err);
    void closeStreamAndBuffer();

    std::vector<std::pair<std::string, std::function<PaError()>>> initFunctions;
    PaStreamParameters inputParams{};
    PaStream* stream{};
    PaDeviceIndex device{paNoDevice};
    uint32_t samplingRate{};
    std::optional<std::vector<int16_t>> buffer;
    bool errorOccured{};
};


