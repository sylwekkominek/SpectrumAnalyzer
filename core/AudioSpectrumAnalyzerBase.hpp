/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */
#pragma once

#include "SpectrumAnalyzerBase.hpp"


class AudioSpectrumAnalyzerBase : public SpectrumAnalyzerBase
{
public:

    using SpectrumAnalyzerBase::SpectrumAnalyzerBase;
    void samplesUpdater() override;
    void drafter() override;
    void flowController() override;
protected:
    std::string audioConfigFile="audioConfig";
};
