/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include "AudioSpectrumAnalyzerBase.hpp"

class StereoRmsMeter : public AudioSpectrumAnalyzerBase
{
public:

    StereoRmsMeter(const Configuration &configuration, std::promise<AppEvent> &&promise = std::promise<AppEvent>());
    void init() override;
    void fftCalculator() override;
    void processing() override;
    ~StereoRmsMeter() override;

protected:

    struct StereoFftData
    {
        FftResult left;
        FftResult right;
    };
};
