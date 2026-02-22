#include "FrequencyTextPositions.hpp"

FrequencyTextPositions::FrequencyTextPositions(const Frequencies &value) : value(value)
{
}

FrequencyTextPositions::FrequencyTextPositions(const ThemeConfig themeConfig) : value(getFrequencyTextPositions(themeConfig))
{
}

std::string FrequencyTextPositions::getInfo()
{
    return std::string(
        R"(//Description: Allows selecting the frequency at which a frequency label can be placed.
//Any value can be entered, but it will be automatically adjusted to the nearest
//calculated FFT frequency, as done in the Frequencies.txt file.
)");
}

std::ostream& operator<<(std::ostream& os, const FrequencyTextPositions &frequencyTextPositions)
{
    os <<"frequencyTextPositions: ";

    for(const auto &el: frequencyTextPositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

Frequencies FrequencyTextPositions::getFrequencyTextPositions(const ThemeConfig themeConfig)
{
    const Frequencies defaultValue{20, 2000.00,  4000.00, 6000.00,  8000.00, 10000.00, 12000.00,  14000.00,  16000.00,  18000.00,  20000.00};

    switch(themeConfig)
    {
        case ThemeConfig::Theme1:
            return {20, 50,125, 250, 500, 1000, 2000.00, 4000, 8000, 16000};
        case ThemeConfig::Theme2:
            return defaultValue;
        case ThemeConfig::Theme3:
            return {20, 2000.00,  4000.00, 6000.00,  8000.00, 10000.00, 12000.00,  14000.00,  16000.00};
        case ThemeConfig::Theme4:
            return {20, 2000.00, 4000.00, 6000.00,  8000.00, 10000.00};
        case ThemeConfig::Theme5:
            return {20, 1000.00, 2000.00, 3000.00, 4000.00, 5000.00};
        case ThemeConfig::Theme6:
            return {20, 500.00, 1000.00,1500, 2000.00};
        case ThemeConfig::Theme7:
            return {20,100,200,300,400, 500};
        default:
            return defaultValue;
    }
}
