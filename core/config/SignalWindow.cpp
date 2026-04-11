#include "SignalWindow.hpp"
#include <cmath>

constexpr double PI = 3.14159265358979323846;

SignalWindow::SignalWindow(const Mode mode, uint16_t numberOfSamples) : mode(mode), value(getSignalWindow(numberOfSamples))
{
}

SignalWindow::SignalWindow(const std::vector<float> &window) :  value(window)
{
}

std::vector<float> SignalWindow::getSignalWindow(uint16_t numberOfSamples)
{
    std::vector<float> window(numberOfSamples);

    for (size_t n = 0; n < numberOfSamples; ++n)
    {
        window[n] = (mode == Mode::StereoRmsMeter) ? 1.0 : (0.5 * (1.0 - std::cos(2.0 * PI * n / (numberOfSamples - 1))));
    }

    return window;
}

std::string SignalWindow::getInfo()
{
    return std::string(
        R"(//Description: Users can apply a custom window function to the signal before frequency analysis, which allows for experimentation with different windowing techniques and their impact on the result. This offers flexibility in shaping the signal to reduce artifacts like spectral leakage. For example, you can use window functions from the scipy.signal module—such as Hamming, Blackman, or Kaiser—to tailor the analysis to your specific needs.
)");
}

std::ostream& operator<<(std::ostream& os, const SignalWindow& signalWindow)
{
    os <<"signalWindow size: "<<signalWindow.value.size()<<std::endl;
    return os;
}
