#include "PythonDataSourceEnabled.hpp"

PythonDataSourceEnabled::PythonDataSourceEnabled(bool value) : value(value)
{
}

PythonDataSourceEnabled::PythonDataSourceEnabled(const ThemeConfig themeConfig) : value(getPythonDataSourceEnabled(themeConfig))
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

bool PythonDataSourceEnabled::getPythonDataSourceEnabled(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return false;
    }
}
