/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "SpectrumAnalyzerBase.hpp"
#include "ConfigReader.hpp"


class AudioSpectrumAnalyzer : public SpectrumAnalyzerBase
{
public:

    AudioSpectrumAnalyzer(const Configuration &configuration);
    virtual void init() override;
    virtual void samplesUpdater() override;
    virtual void fftCalculator() override;
    virtual void processing() override;
    virtual void drafter() override;
    virtual void flowController() override;
    virtual ~AudioSpectrumAnalyzer() override;
protected:
    std::string audioConfigFile="audioConfig";
};
