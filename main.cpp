/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/AudioSpectrumAnalyzer.hpp"
#include "core/StereoRmsMeter.hpp"
#include "core/ConfigReader.hpp"
#include <iostream>

void printLicense()
{
    const std::string license = R"(Copyright (C) 2024-2026, Sylwester Kominek

Source code: https://github.com/sylwekkominek/SpectrumAnalyzer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>



PRESS 0–9 TO CHANGE THE COLOR THEME
)";

    std::cout<<license<<std::endl;
}

void updateStates(ThemeConfig &theme, ApplicationState &state, const AppEvent &event)
{
    std::visit([&](auto&& v)
    {
        using T = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<T, ThemeConfig>)
        {
            theme = v;
        }
        else if constexpr (std::is_same_v<T, ApplicationState>)
        {
            state = v;
        }
    }, event);
}

std::unique_ptr<SpectrumAnalyzerBase> createAnalyzer(Mode mode, const Configuration &config)
{
    switch(mode)
    {
        case Mode::StereoRmsMeter:
            return std::make_unique<StereoRmsMeter>(config);
        default:
            return std::make_unique<AudioSpectrumAnalyzer>(config);
    }
}

int main()
{
    printLicense();

    const Mode mode = Mode::Analyzer;
    ThemeConfig currentTheme = ThemeConfig::Theme1;
    ApplicationState currentAppState = ApplicationState::Running;


    while (currentAppState != ApplicationState::Shutdown)
    {
        ConfigReader configReader(currentTheme, mode);
        const Configuration &config = configReader.getConfig();
        std::cout << config << std::endl;

        std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = createAnalyzer(mode, config);

        spectrumAnalyzer->init();
        spectrumAnalyzer->run();

        AppEvent event = spectrumAnalyzer->getEvent();
        updateStates(currentTheme, currentAppState, event);
    }


    return 0;
}
