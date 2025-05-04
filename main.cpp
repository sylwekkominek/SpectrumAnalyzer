/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/AudioSpectrumAnalyzer.hpp"
#include "core/ConfigReader.hpp"
#include <iostream>

void printLicense()
{
    const std::string license = R"(Copyright (C) 2024-2025, Sylwester Kominek

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

)";

    std::cout<<license<<std::endl;
}


int main()
{
    printLicense();

    ConfigReader configReader;
    const Configuration &config = configReader.getConfig();
    std::cout<<"Configuration read out from config.py: "<<std::endl;
    std::cout<<config<<std::endl;

    std::unique_ptr<SpectrumAnalyzerBase> spectrumAnalyzer = std::make_unique<AudioSpectrumAnalyzer>(config);

    spectrumAnalyzer->init();
    spectrumAnalyzer->run();

    return 0;
}
