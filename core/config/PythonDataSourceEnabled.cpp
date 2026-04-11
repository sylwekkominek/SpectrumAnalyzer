#include "PythonDataSourceEnabled.hpp"

PythonDataSourceEnabled::PythonDataSourceEnabled(bool value) : value(value)
{
}

std::string PythonDataSourceEnabled::getInfo()
{
    return std::string(
        R"()");
}

std::ostream& operator<<(std::ostream& os, const PythonDataSourceEnabled &pythonDataSourceEnabled)
{
    os <<"pythonDataSourceEnabled: "<<pythonDataSourceEnabled.value<<std::endl;
    return os;
}

template<>
bool PythonDataSourceEnabled::getPythonDataSourceEnabled<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

template<>
bool PythonDataSourceEnabled::getPythonDataSourceEnabled<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

template<>
bool PythonDataSourceEnabled::getPythonDataSourceEnabled<Mode::StereoRmsMeter>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}

PythonDataSourceEnabled::PythonDataSourceEnabled(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
        case Mode::Analyzer:
            value = getPythonDataSourceEnabled<Mode::Analyzer>(themeConfig);
            break;
        case Mode::Visualizer:
            value = getPythonDataSourceEnabled<Mode::Visualizer>(themeConfig);
            break;
        case Mode::StereoRmsMeter:
            value = getPythonDataSourceEnabled<Mode::StereoRmsMeter>(themeConfig);
            break;
    }
}
