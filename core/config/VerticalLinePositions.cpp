#include "VerticalLinePositions.hpp"

VerticalLinePositions::VerticalLinePositions(const Frequencies &value) : value(value)
{
}

std::string VerticalLinePositions::getInfo()
{
    return std::string(
        R"(//Description: Allows selecting the frequency at which a vertical line can be placed.
//Any frequency value can be entered, but it will be automatically adjusted to the nearest
//calculated FFT frequency, in the same way as in the Frequencies.txt file.
)");
}

std::ostream& operator<<(std::ostream& os, const VerticalLinePositions &verticalLinePositions)
{
    os <<"verticalLinePositions: ";

    for(const auto &el: verticalLinePositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

template<>
Frequencies VerticalLinePositions::getVerticalLinePositions<Mode::Analyzer>(const ThemeConfig themeConfig)
{
    const Frequencies defaultValue{20, 1000.00, 2000.00, 3000.00, 4000.00, 5000.00, 6000.00, 7000.00, 8000.00, 9000.00, 10000.00, 11000.00, 12000.00, 13000.00, 14000.00,  15000.00, 16000.00, 17000.00, 18000.00, 19000.00, 20000.00};

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return {20, 50,125, 250, 500, 1000, 2000, 4000, 8000, 16000};
        case ThemeConfig::Theme2:
            return defaultValue;
        case ThemeConfig::Theme3:
            return {20, 1000.00, 2000.00, 3000.00, 4000.00, 5000.00, 6000.00, 7000.00, 8000.00, 9000.00, 10000.00, 11000.00, 12000.00, 13000.00, 14000.00,  15000.00, 16000.00};
        case ThemeConfig::Theme4:
            return {20,1000.00, 2000.00, 3000.00, 4000.00, 5000.00, 6000.00, 7000.00, 8000.00, 9000.00, 10000.00};
        case ThemeConfig::Theme5:
            return {20, 500, 1000.00,1500.00, 2000.00, 2500.00,3000.00, 3500.00, 4000.00, 4500.00, 5000.00};
        case ThemeConfig::Theme6:
            return {20, 250, 500, 750, 1000.00, 1250, 1500.00, 1750, 2000.00};
        case ThemeConfig::Theme7:
            return {20,100,200,300,400, 500};
        case ThemeConfig::Theme8:
            return {20, 50,125, 250, 500, 1000, 2000, 4000, 8000, 16000};
        case ThemeConfig::Theme9:
            return defaultValue;
        case ThemeConfig::Theme10:
            return {20, 1000.00, 2000.00, 3000.00, 4000.00, 5000.00, 6000.00, 7000.00, 8000.00, 9000.00, 10000.00, 11000.00, 12000.00, 13000.00, 14000.00,  15000.00, 16000.00};

        default:
            return defaultValue;
    }
}

template<>
Frequencies VerticalLinePositions::getVerticalLinePositions<Mode::Visualizer>(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
        default:
            return {20, 50,125, 250, 500, 1000, 2000, 4000, 8000, 16000};;
    }
}

VerticalLinePositions::VerticalLinePositions(const ThemeConfig themeConfig, const Mode mode)
{
    switch(mode)
    {
    case Mode::Analyzer:
        value = getVerticalLinePositions<Mode::Analyzer>(themeConfig);
        break;
    case Mode::Visualizer:
        value = getVerticalLinePositions<Mode::Visualizer>(themeConfig);
        break;
    }
}

