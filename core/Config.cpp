/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "Config.hpp"
#include <numeric>
#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

SignalWindow::SignalWindow(uint16_t numberOfSamples) : value(getSignalWindow(numberOfSamples))
{
}

SignalWindow::SignalWindow(const std::vector<float> &window) : value(window)
{
}

std::vector<float> SignalWindow::getSignalWindow(uint16_t numberOfSamples)
{
    std::vector<float> window(numberOfSamples);

    for (size_t n = 0; n < numberOfSamples; ++n)
    {
        window[n] = 0.5 * (1.0 - std::cos(2.0 * PI * n / (numberOfSamples - 1)));
    }

    return window;
}

ScalingFactor::ScalingFactor(const float value) : value(value)
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

AdvancedColorSettings::AdvancedColorSettings():
    value(getAdvancedColorSettings())
{
}

AdvancedColorSettings::AdvancedColorSettings(const std::string &shader) : value(shader)
{
}

std::string AdvancedColorSettings::getAdvancedColorSettings()
{
        return R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform vec2 boundary; // xBegin, xEnd
uniform float bass;
vec3 nebulaBarColor(float y)
{
    vec3 violet  = vec3(0.4, 0.2, 0.7);
    vec3 magenta = vec3(0.8, 0.1, 0.6);
    vec3 red     = vec3(0.9, 0.2, 0.2);
    vec3 orange  = vec3(1.0, 0.5, 0.1);
    vec3 color = mix(violet, magenta, smoothstep(0.0, 0.33, y));
    color = mix(color, red, smoothstep(0.33, 0.66, y));
    color = mix(color, orange, smoothstep(0.66, 1.0, y));
    return color;
}
void main()
{
    float t = clamp(calculatedPosition.y, -1.0, 1.0);
    float y = (t + 1.0) * 0.5;
    vec3 barColor = nebulaBarColor(y);
    vec4 baseColor = mix(vec4(barColor, 1.0), vertColor, 0.4);
    bool insideBoundary = calculatedPosition.x > boundary.x && calculatedPosition.x < boundary.y;
    if (insideBoundary)
    {
        baseColor.rgb = min(baseColor.rgb * 1.5, vec3(1.0));
    }
    Color = baseColor;
})";
}

BackgroundColorSettings::BackgroundColorSettings():
        value(getBackgroundColorSettings())
{
}

BackgroundColorSettings::BackgroundColorSettings(const std::string &shader) : value(shader)
{
}

std::string BackgroundColorSettings::getBackgroundColorSettings()
{
        return R"(#version 330 core
in vec4 calculatedPosition;
in vec4 vertColor;
out vec4 Color;
uniform float timeInMilliSeconds;
uniform float bass;
float noise(vec2 p) {
    return sin(p.x * 3.0) * cos(p.y * 2.0);
}
vec3 nebulaGlow(float x)
{
    return mix(vec3(0.05, 0.05, 0.1), vec3(0.3, 0.2, 0.5), smoothstep(0.0, 1.0, x));
}
void main()
{
    float time = timeInMilliSeconds / 1000.0;
    vec2 uv = calculatedPosition.xy;
    vec2 warped = uv * 2.0 + vec2(
        cos(time * 0.3 + uv.y * 6.0),
        sin(time * 0.4 + uv.x * 5.0)
    );
    float pattern = noise(warped + time * 0.5) * 0.5 + 0.5;
    float pulse = 0.6 + 0.3 * sin(time * 0.7 + length(uv) * 2.0);
    pattern *= pulse;
    vec3 glow = nebulaGlow(pattern);
    float y = (clamp(calculatedPosition.y, -1.0, 1.0) + 1.0) * 0.5;
    vec3 gradient = mix(vec3(0.01, 0.01, 0.02), vec3(0.1, 0.1, 0.2), y);
    vec3 final = mix(gradient, glow, 0.6);
    Color = vec4(final, 1.0);
})";
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

std::ostream& operator<<(std::ostream& os, const MaxQueueSize &maxQueueSize)
{
    os <<"maxQueueSize: "<<maxQueueSize.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PythonDataSourceEnabled &pythonDataSourceEnabled)
{
    os <<"pythonDataSourceEnabled: "<<pythonDataSourceEnabled.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DefaultFullscreenState& defaultFullscreenState)
{
    os <<"defaultFullscreenState: "<<defaultFullscreenState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfRectangles &numberOfRectangles)
{
    os <<"numberOfRectangles: "<<numberOfRectangles.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const GapWidthInRelationToRectangleWidth& gapWidthInRelationToRectangleWidth)
{
    os <<"gapWidthInRelationToRectangleWidth: "<<gapWidthInRelationToRectangleWidth.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const MaximizedWindowSize &maximizedWindowSize)
{
    os <<"maximizedWindowSize: "<<maximizedWindowSize.value.first<<" "<<maximizedWindowSize.value.second<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NormalWindowSize &normalWindowSize)
{
    os <<"maximizedWindowSize: "<<normalWindowSize.value.first<<" "<<normalWindowSize.value.second<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSamples &numberOfSamples)
{
    os <<"numberOfSamples: "<<numberOfSamples.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const SamplingRate &samplingRate)
{
    os <<"samplingRate: "<<samplingRate.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DesiredFrameRate &desiredFrameRate)
{
    os <<"desiredFrameRate: "<<desiredFrameRate.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForAveraging &numberOfSignalsForAveraging)
{
    os <<"numberOfSignalsForAveraging: "<<numberOfSignalsForAveraging.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const NumberOfSignalsForMaxHold &numberOfSignalsForMaxHold)
{
    os <<"numberOfSignalsForMaxHold: "<<numberOfSignalsForMaxHold.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const AlphaFactor &alphaFactor)
{
    os <<"alphaFactor: "<<alphaFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const ScalingFactor &scalingFactor)
{
    os <<"scalingFactor: "<<scalingFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const OffsetFactor &offsetFactor)
{
    os <<"offsetFactor: "<<offsetFactor.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldVisibilityState &dynamicMaxHoldVisibilityState)
{
    os <<"dynamicMaxHoldVisibilityState: "<<dynamicMaxHoldVisibilityState.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldRectangleHeightInPercentOfScreenSize &dynamicMaxHoldRectangleHeightInPercentOfScreenSize)
{
    os <<"dynamicMaxHoldRectangleHeightInPercentOfScreenSize: "<<dynamicMaxHoldRectangleHeightInPercentOfScreenSize.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldSpeedOfFalling &dynamicMaxHoldSpeedOfFalling)
{
    os <<"dynamicMaxHoldSpeedOfFalling: "<<dynamicMaxHoldSpeedOfFalling.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DynamicMaxHoldAccelerationStateOfFalling & dynamicMaxHoldAccelerationStateOfFalling)
{
    os <<"dynamicMaxHoldAccelerationStateOfFalling: "<<dynamicMaxHoldAccelerationStateOfFalling.value<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const HorizontalLinePositions &horizontalLinePositions)
{
    os <<"horizontalLinePositions: ";

    for(const auto &el: horizontalLinePositions.value)
    {
        os <<el<<" ";
    }
    os <<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorOfStaticLines &colorOfStaticLines)
{
    os<<"colorOfStaticLines: ";
    for(auto & component: colorOfStaticLines.value)
    {
        os<<component<<" ";
    }
    os<<std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorsOfRectangle &colorsOfRectangle)
{
    os <<"colorsOfRectangle: "<<std::endl;
    for(auto &[vertex, color]: colorsOfRectangle.value)
    {
        os<<"vertex: "<<vertex<<" color: ";
        for(auto & component: color)
        {
            os<<component<<" ";
        }
        os<<std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const ColorsOfDynamicMaxHoldRectangle &colorsOfDynamicMaxHoldRectangle)
{
    os <<"colorsOfDynamicMaxHoldRectangle: "<<std::endl;
    for(auto &[vertex, color]: colorsOfDynamicMaxHoldRectangle.value)
    {
        os<<"vertex: "<<vertex<<" color: ";
        for(auto & component: color)
        {
            os<<component<<" ";
        }
        os<<std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SignalWindow& signalWindow)
{
    os <<"signalWindow size: "<<signalWindow.value.size()<<std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Configuration &config)
{
    os<<config.data.get<PythonDataSourceEnabled>();
    os<<config.data.get<DefaultFullscreenState>();
    os<<config.data.get<MaximizedWindowSize>();
    os<<config.data.get<NormalWindowSize>();
    os<<config.data.get<NumberOfRectangles>();
    os<<config.data.get<GapWidthInRelationToRectangleWidth>();
    os<<config.data.get<NumberOfSamples>();
    os<<config.data.get<SamplingRate>();
    os<<config.data.get<DesiredFrameRate>();
    os<<config.data.get<NumberOfSignalsForAveraging>();
    os<<config.data.get<NumberOfSignalsForMaxHold>();
    os<<config.data.get<AlphaFactor>();
    os<<config.data.get<MaxQueueSize>();
    os<<config.data.get<ScalingFactor>();
    os<<config.data.get<OffsetFactor>();
    os<<config.data.get<DynamicMaxHoldVisibilityState>();
    os<<config.data.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>();
    os<<config.data.get<DynamicMaxHoldAccelerationStateOfFalling>();
    os<<config.data.get<Freqs>();
    os<<config.data.get<SignalWindow>();
    os<<config.data.get<HorizontalLinePositions>();
    os<<config.data.get<ColorOfStaticLines>();
    os<<config.data.get<ColorsOfRectangle>();
    os<<config.data.get<ColorsOfDynamicMaxHoldRectangle>();

    return os;
}

std::string Freqs::getInfo()
{
    return std::string(
R"(//Description: This function returns a list of target frequencies (in Hz) that the application will attempt to visualize using vertical bars on the screen. However, it’s important to understand that the actual signal analysis is carried out using an FFT (Fast Fourier Transform), whose frequency resolution is determined by two key parameters: the number of samples (getNumberOfSamples()) and the sampling rate (getSamplingRate()). For example, if you use 4096 samples with a sampling rate of 44100 Hz, the frequency resolution of the FFT is approximately 10.77 Hz (calculated by dividing the sampling rate by the number of samples, i.e., 44100 / 4096). This means that the FFT output only includes energy values at specific, evenly spaced frequency steps — also known as FFT bins — such as 0 Hz, 10.77 Hz, 21.55 Hz, 32.32 Hz, 43.09 Hz, and so on. When you define a list of frequencies using getFrequencies(), the application maps each requested frequency to the nearest available FFT bin. As a result, if multiple frequencies are too close together (for example, 40 Hz, 41 Hz, and 43 Hz), they may all get mapped to the same bin — such as 43.09 Hz — and thus produce identical or nearly identical visual bars on the screen. The number of bars shown in the visualization is equal to the number of frequencies returned by this function, but this does not improve the frequency resolution of the signal analysis. The FFT is always computed over the full spectrum based on the sample size and sampling rate. Increasing the number of requested frequencies only determines how many bars are drawn and which FFT bins are sampled — not how precisely the signal is analyzed.
//Default value: (20 40 60 ... When analyzing sound, the function usually returns a list of frequencies where low frequencies are represented by closely spaced values to better distinguish subtle differences, while higher frequencies are spaced farther apart since fine details are harder to perceive. However, this approach is common but not mandatory — the frequency spacing can be adjusted based on specific needs or preferences.)
)");
}

std::string SignalWindow::getInfo()
{
    return std::string(
R"(//Description: Users can apply a custom window function to the signal before frequency analysis, which allows for experimentation with different windowing techniques and their impact on the result. This offers flexibility in shaping the signal to reduce artifacts like spectral leakage. For example, you can use window functions from the scipy.signal module—such as Hamming, Blackman, or Kaiser—to tailor the analysis to your specific needs.
//Default value: Hanning window
)");
}

std::string PythonDataSourceEnabled::getInfo()
{
    return std::string(
        R"()");
}

std::string DefaultFullscreenState::getInfo()
{
    return std::string(
R"(//Description: Determines whether the SpectrumAnalyzer application starts in fullscreen mode by default. Users can choose this to enable fullscreen startup automatically. This is especially useful on devices like Raspberry Pi, where fullscreen improves user experience and system integration.
//Default value: False (starts in windowed mode)
)");
}

std::string NumberOfRectangles::getInfo()
{
    return std::string();
}

std::string GapWidthInRelationToRectangleWidth::getInfo()
{
    return std::string();
}

std::string MaximizedWindowSize::getInfo()
{
    return std::string(
R"(//Description: The user can select the fullscreen resolution. This is particularly useful on devices like the Raspberry Pi, where the maximum resolution may cause performance issues. Allowing users to lower the resolution can improve application responsiveness and ensure smoother performance.
//Default value: 1920×1080
)");
}

std::string NormalWindowSize::getInfo()
{
    return std::string(
R"(//Description: The initial window size when the application starts in windowed mode. This setting ensures that the strips always look the way the user wants, providing a consistent visual layout without the need for manual resizing after launch.
//Default value: 1280×512
)");
}

std::string NumberOfSamples::getInfo()
{
    return std::string(
R"(//Description: Users can adjust the number of audio samples processed each time to find the best setup for them. Using fewer than 4096 samples makes the display smoother and more responsive but lowers the detail at low frequencies. For example, with fewer samples, it becomes harder to distinguish between very close frequencies like 20 Hz and 30 Hz. Increasing the number of samples improves frequency detail but can make the program less responsive.
//Default value: 4096
)");
}

std::string SamplingRate::getInfo()
{
    return std::string(
R"(//Description: This parameter is rarely changed but can be useful when analyzing audio at non-standard sample rates. It also allows experimenting with oversampling or downsampling (which PyAudio supports). Additionally, you might need to adjust it when connecting external devices with different sampling frequencies.
//Default value: 44100 Hz
)");
}

std::string DesiredFrameRate::getInfo()
{
    return std::string(
R"(//Description: Sets the target number of frames per second (FPS) displayed on the screen. Since the number of samples and sampling rate limit how frequently new data can be processed, the program increases the frame rate by using overlapping. For example, with 4096 samples at a 44100 Hz sampling rate, the default frame rate is about 11 FPS. To reach higher rates like 60 FPS, the program applies Welch’s method, processing overlapping segments of the signal to effectively analyze more data per second. This parameter also allows adjusting the application to run smoothly on hardware with limited performance, such as a Raspberry Pi, which may struggle with higher frame rates, especially at higher resolutions.
//Default value: 60 FPS
)");
}

std::string NumberOfSignalsForAveraging::getInfo()
{
    return std::string(
R"(//Description: This parameter controls how many signal samples are averaged together to produce the displayed output. Increasing the number of signals for averaging results in smoother, more stable visualizations by reducing short-term fluctuations. However, it also introduces a slight delay in responsiveness. Lower values make the output more immediate and reactive but can appear more “jumpy.”
//Default value: 1
)");
}

std::string NumberOfSignalsForMaxHold::getInfo()
{
    return std::string(
R"(//Description: Defines how many of the recent signal frames are considered when calculating the maximum hold value. This affects how quickly the displayed peak values update and start to drop, making the peak indicators appear either more stable or more dynamic.
//Default value: 5
)");
}

std::string AlphaFactor::getInfo()
{
    return std::string(
R"(//Description: Controls the amount of smoothing applied to the signal visualization. A lower alpha factor results in a smoother and more stable display, but with slower response to rapid changes. A higher alpha makes the visualization more responsive and sensitive to signal variations, although it may result in a more flickery or unstable display.
//Default value: 0.25
)");
}

std::string MaxQueueSize::getInfo()
{
    return std::string(
        R"()");
}

std::string ScalingFactor::getInfo()
{
    return std::string(
R"(//Description: Adjusts the amplitude of the signal after windowing to ensure the signal level remains accurate, since different window functions can alter the overall amplitude.
//Default value: Calculates and returns the amplitude correction based on the average value of the window.
)");
}

std::string OffsetFactor::getInfo()
{
    return std::string(
R"(//Description: Allows precise adjustment of the signal amplitude, useful when working with external devices or simply to raise the signal level if the displayed bars on the screen appear too low.
//Default value: 0
)");
}

std::string DynamicMaxHoldVisibilityState::getInfo()
{
    return std::string(
R"(//Description: Toggles the visibility of on-screen elements that display the peak (max hold) values over time. When enabled, small markers show the highest recent signal levels.
//Default value: True
)");
}

std::string DynamicMaxHoldRectangleHeightInPercentOfScreenSize::getInfo()
{
    return std::string(
R"(//Description: Allows the user to control the height of the rectangles that represent dynamic max hold values. The value is given as a percentage of the screen height — for example, 1 means the rectangle will be 1% of the total screen height.
//Default value: 0.8
)");
}

std::string DynamicMaxHoldSpeedOfFalling::getInfo()
{
    return std::string(
R"(//Description: Sets how fast the max hold markers decrease in value, affecting the speed at which the peak indicators move downward.
//Default value: 900
)");
}

std::string DynamicMaxHoldAccelerationStateOfFalling::getInfo()
{
    return std::string(
R"(//Description: Determines whether the max hold elements fall with acceleration or at a constant speed. This gives the user control over the visual behavior and final effect of the peak indicators.
//Default value: True
)");
}

std::string HorizontalLinePositions::getInfo()
{
    return std::string(
R"(//Description: Lets the user define how many horizontal lines are shown on the display and at which signal power levels (in dBFS) they appear. This allows for precise customization of the scale and visual reference points in the signal visualization.
//Default value: –6.02, –12.04, –18.06, ..., –90.30 dBFS
)");
}

std::string ColorOfStaticLines::getInfo()
{
    return std::string(
R"(//Description: Allows you to customize the color (RGB) and transparency of static lines that remain constant on the display.
//Default value: (0.15,0.15,0.15,0.15)
)");
}

std::string ColorsOfRectangle::getInfo()
{
    return std::string(
R"(//Description: Each bar (rectangle) on the screen is made up of 4 corners (vertices). You can set a separate color for each vertex using RGBA values — where R is red, G is green, B is blue, and A stands for transparency. The GPU then smoothly blends the colors between the corners, allowing you to easily control the appearance of each bar, for example by creating smooth color gradients from top to bottom.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::string ColorsOfDynamicMaxHoldRectangle::getInfo()
{
    return std::string(
        R"(//Description: Uses the same method of assigning RGBA colors to each of the 4 rectangle vertices as for the main bars. This time, however, it applies to the small visual elements that hold and display the peak (max hold) values over time. By setting the vertex colors, you can control the appearance of these peak indicators, including gradients and transparency.
//Default value: RGBA color (Red, Green, Blue, Transparency) for each vertex
)");
}

std::string AdvancedColorSettings::getInfo()
{
    return std::string(
R"(//Description: The application first determines how many frequency bins are defined by the user, based on the number of frequencies provided by the getFrequencies() function. Then, it divides the screen into the same number of vertical bars — one for each frequency. Each bar is built using two triangles (6 vertices), which form a rectangle defined by 4 unique corner points in 2D space (x, y). These bars are then sent to the GPU with their initial positions. At this stage, all bars are evenly spaced and aligned. As the application analyzes the audio (or other signal) in real time, it calculates the power of each frequency. Based on this power, the height or vertical position of each bar is updated dynamically to reflect the current signal strength. The vertex shader updates the positions of the bar's corners depending on the signal’s power for that frequency. The GPU then fills in all the pixels between those corners by generating fragments — and each of these fragments is processed by the fragment shader. One of the values the fragment shader receives is calculatedPosition, which (in simplified terms) tells it how high up the current pixel is within its rectangle. This vertical position is used to control how the pixel will be colored. In addition, the shader receives vertColor values — these are colors assigned to each corner of the rectangle using application-side functions like getColorsOfRectangle() and getColorsOfDynamicMaxHoldRectangle(). The GPU automatically blends these colors across the surface of the rectangle, so each pixel gets its own color depending on where it lies. Inside the fragment shader, this blended color is combined with a vertical color gradient that transitions from blue → cyan → green → yellow → red, depending on the pixel’s height within the bar. This results in vibrant and informative color transitions that follow the signal in real time. Users can adjust the rectangle corner colors directly in the application to experiment with color effects. And because the shader computes colors for each pixel independently, there’s a lot of flexibility for customization. More advanced users can even edit the shader code to fully control how colors behave. All of this runs on the GPU, which executes the fragment shader for every pixel of every bar in parallel. This makes the visualization extremely fast and smooth — even when rendering many bars at once. This method not only results in dynamic and responsive visualizations, but also allows users to experiment and learn how shaders and GPU parallelism work in real-time graphics.
//if you get errors after modification please find following log with error msg: "VS log:"
)");
}

std::string BackgroundColorSettings::getInfo()
{
    return std::string(
        R"()");
}
