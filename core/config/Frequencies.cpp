#include "Frequencies.hpp"
#include <vector>
#include <cmath>

Freqs::Freqs(const Frequencies &value) : value(value)
{
}

Freqs::Freqs(const ThemeConfig themeConfig, uint16_t sampleRate, uint16_t fftSize) : sampleRate(sampleRate), fftSize(fftSize)
{
    value=getFreqs(themeConfig);
}

std::string Freqs::getInfo()
{
    return std::string(
        R"(//Description: This function returns a list of target frequencies (in Hz) that the application will attempt to visualize using vertical bars on the screen. However, it’s important to understand that the actual signal analysis is carried out using an FFT (Fast Fourier Transform), whose frequency resolution is determined by two key parameters: the number of samples (getNumberOfSamples()) and the sampling rate (getSamplingRate()). For example, if you use 4096 samples with a sampling rate of 44100 Hz, the frequency resolution of the FFT is approximately 10.77 Hz (calculated by dividing the sampling rate by the number of samples, i.e., 44100 / 4096). This means that the FFT output only includes energy values at specific, evenly spaced frequency steps — also known as FFT bins — such as 0 Hz, 10.77 Hz, 21.55 Hz, 32.32 Hz, 43.09 Hz, and so on. When you define a list of frequencies using getFrequencies(), the application maps each requested frequency to the nearest available FFT bin. As a result, if multiple frequencies are too close together (for example, 40 Hz, 41 Hz, and 43 Hz), they may all get mapped to the same bin — such as 43.09 Hz — and thus produce identical or nearly identical visual bars on the screen. The number of bars shown in the visualization is equal to the number of frequencies returned by this function, but this does not improve the frequency resolution of the signal analysis. The FFT is always computed over the full spectrum based on the sample size and sampling rate. Increasing the number of requested frequencies only determines how many bars are drawn and which FFT bins are sampled — not how precisely the signal is analyzed.
//Default value: (20 40 60 ... When analyzing sound, the function usually returns a list of frequencies where low frequencies are represented by closely spaced values to better distinguish subtle differences, while higher frequencies are spaced farther apart since fine details are harder to perceive. However, this approach is common but not mandatory — the frequency spacing can be adjusted based on specific needs or preferences.)
)");
}


std::ostream& operator<<(std::ostream& os, const Freqs &freqs)
{
    os <<"frequencies: ";

    for(const auto &el: freqs.value)
    {
        os <<el<<" ";
    }

    os <<std::endl;

    return os;
}

Frequencies generateFrequencies(double sampleRate, uint16_t fftSize, double startFrequency,double endFrequency)
{
    const double stepHz = sampleRate / fftSize;

    const int startIndex = static_cast<int>(std::ceil(startFrequency / stepHz));
    const int endIndex   = static_cast<int>(std::floor(endFrequency / stepHz));

    std::vector<float> freqs;
    freqs.reserve(std::max(0, endIndex - startIndex + 1));

    for (int i = startIndex; i <= endIndex; ++i)
    {
        freqs.push_back(i * stepHz);
    }

    return freqs;
}

Frequencies getLogFrequencies()
{
    return Frequencies{20.000, 25.383, 31.000, 36.147, 42.000, 50, 63.000, 71.429, 80.000, 91.000, 125.000, 135.714, 146.429, 160.000, 180.000, 250.000, 270.000, 290.000, 320.000, 360.000, 500.000, 540.000, 580.000, 640.000, 720.000, 1000.000, 1080.000, 1160.000, 1280.000, 1440.000, 2000.000, 2160.000, 2320.000, 2560.000, 2880.000, 4000.000, 4320.000, 4640.000, 5120.000, 5760.000, 8000.000, 8979.696, 10079.368, 11313.708, 12699.208, 14254.379, 16000.000};
}

Frequencies Freqs::getFreqs(const ThemeConfig themeConfig)
{
    switch(themeConfig)
    {
    case ThemeConfig::Theme1:
        return getLogFrequencies();
    case ThemeConfig::Theme2:
        return generateFrequencies(sampleRate,fftSize, 20, 20020);
    case ThemeConfig::Theme3:
        return generateFrequencies(sampleRate,fftSize, 20, 16020);
    case ThemeConfig::Theme4:
        return generateFrequencies(sampleRate,fftSize, 20, 10010);
    case ThemeConfig::Theme5:
        return generateFrequencies(sampleRate,fftSize, 6, 5006);
    case ThemeConfig::Theme6:
        return generateFrequencies(sampleRate,fftSize, 6, 2004);
    case ThemeConfig::Theme7:
        return generateFrequencies(sampleRate,fftSize, 3, 501);
    default:
        return generateFrequencies(sampleRate,fftSize, 20, 20020);
    }
}
