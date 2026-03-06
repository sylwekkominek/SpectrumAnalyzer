/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "../CommonTypes.hpp"
#include <string>
#include <ostream>

struct PythonDataSourceEnabled
{
    PythonDataSourceEnabled(bool value);
    PythonDataSourceEnabled(const ThemeConfig themeConfig, const Mode mode);
    std::string getInfo();
    bool value;
    const std::string name{"PythonDataSourceEnabled"};
private:
    template <Mode>
    bool getPythonDataSourceEnabled(const ThemeConfig themeConfig);
};

std::ostream& operator<<(std::ostream& os, const PythonDataSourceEnabled &pythonDataSourceEnabled);

