#include "ScalingFactor.hpp"
#include <numeric>

ScalingFactor::ScalingFactor(float value) : value(value)
{
}

ScalingFactor::ScalingFactor(const std::vector<float> &window) : value(getScalingFactor(window))
{
}

float ScalingFactor::getScalingFactor(const std::vector<float> &window)
{
    const float windowSum = std::accumulate(window.begin(), window.end(), 0.0);
    const float windowAverage = windowSum / window.size();
    const float amplitudeCorrection = 1.0 / windowAverage;
    return amplitudeCorrection;
}


std::string ScalingFactor::getInfo()
{
    return std::string(
        R"(//Description: Adjusts the amplitude of the signal after windowing to ensure the signal level remains accurate, since different window functions can alter the overall amplitude.
//Default value: Calculates and returns the amplitude correction based on the average value of the window.
)");
}

std::ostream& operator<<(std::ostream& os, const ScalingFactor &scalingFactor)
{
    os <<"scalingFactor: "<<scalingFactor.value<<std::endl;
    return os;
}
