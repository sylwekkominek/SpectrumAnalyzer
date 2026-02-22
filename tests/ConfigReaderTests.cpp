/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/ConfigReader.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class ConfigReaderTests : public ValuesChecker<-6>, public ::testing::Test
{
public:
    const std::string testFolderName = "configReaderTests";
    const ThemeConfig theme = ThemeConfig::Theme1;

    const float precision = 1e-6;
    const float lowPrecision = 1e-2;

    const std::string comment ="//Some not meaningful comment";

    const ColorsOfRectanglePerVertices colors{
        {0,{0.2, 0.2, 0.2,0.1}},
        {1,{0,0,0,0.1}},
        {2,{0,0,0,0.1}},
        {3,{0.2, 0.2, 0.2,0.1}}
    };

    const ColorsOfRectanglePerVertices transparentColors{
        {0,{0.2, 0.2, 0.2,0.25}},
        {1,{0,0,0,0.25}},
        {2,{0,0,0,0.25}},
        {3,{0.2, 0.2, 0.2,0.25}}
    };

    void checkDefaultConfig(const Configuration &config)
    {
        EXPECT_EQ(config.get<PythonDataSourceEnabled>(), false);
        EXPECT_EQ(config.get<DefaultFullscreenState>(), false);
        EXPECT_EQ(config.get<MaximizedWindowSize>().first, 1920);
        EXPECT_EQ(config.get<MaximizedWindowSize>().second, 1080);
        EXPECT_EQ(config.get<NormalWindowSize>().first, 1280);
        EXPECT_EQ(config.get<NormalWindowSize>().second, 512);
        EXPECT_EQ(config.get<NumberOfSamples>(), 8192);
        EXPECT_EQ(config.get<SamplingRate>(), 48000);
        EXPECT_EQ(config.get<DesiredFrameRate>(), 55);
        EXPECT_EQ(config.get<GapWidthInRelationToRectangleWidth>(), 0);
        EXPECT_EQ(config.get<NumberOfSignalsForAveraging>(), 1);
        EXPECT_EQ(config.get<NumberOfSignalsForMaxHold>(), 5);
        EXPECT_EQ(config.get<MaxQueueSize>(), 20);
        EXPECT_NEAR(config.get<AlphaFactor>(), 0.25, precision);
        EXPECT_NEAR(config.get<ScalingFactor>(), 2.000244, precision);
        EXPECT_NEAR(config.get<OffsetFactor>(), 0, precision);
        EXPECT_NEAR(config.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>(), 0.8, precision);
        EXPECT_NEAR(config.get<DynamicMaxHoldSpeedOfFalling>(), 900, lowPrecision);
        EXPECT_NEAR(config.get<DynamicMaxHoldSecondarySpeedOfFalling>(), 1000, lowPrecision);
        EXPECT_EQ(config.get<DynamicMaxHoldAccelerationStateOfFalling>(), true);
        EXPECT_EQ(config.get<DynamicMaxHoldVisibilityState>(), true);
        EXPECT_EQ(config.get<AdvancedColorSettings>().size(), 1397);
        EXPECT_EQ(config.get<BackgroundColorSettings>().size(), 1018);
        valueChecker(config.get<ColorOfStaticLines>(),  Color{0.2,0.2,0.2,1});
        valueChecker(config.get<HorizontalLinePositions>(),  Positions{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30});
        valueChecker(config.get<Freqs>(), Frequencies{20.000, 25.4, 31.000, 36.1, 42, 50, 63, 71.4, 80, 91, 125, 135.7, 146.4, 160, 180, 250, 270, 290, 320, 360, 500, 540, 580, 640, 720, 1000, 1080, 1160, 1280, 1440, 2000, 2160, 2320, 2560, 2880, 4000, 4320, 4640, 5120, 5760, 8000, 8979.7, 10079.4, 11313.7, 12699.2, 14254.4, 16000.000});
        EXPECT_EQ(config.get<SignalWindow>().size(), 8192);
        positionValuesChecker(colors, config.get<ColorsOfRectangle>());
        positionValuesChecker(colors, config.get<ColorsOfDynamicMaxHoldRectangle>());
        positionValuesChecker(transparentColors, config.get<ColorsOfDynamicMaxHoldSecondaryRectangle>());
        valueChecker(config.get<ColorOfDynamicMaxHoldLine>(),  Color{0.15, 1.00, 0.15, 1.00});
        valueChecker(config.get<ColorOfDynamicMaxHoldSecondaryLine>(),  Color{0.70, 0.70, 0.70, 1.00});
        valueChecker(config.get<ColorOfStaticText>(),  Color{0.50, 0.50, 0.50, 1.00});
        EXPECT_EQ(config.get<LinesVisibilityState>(),  false);
        EXPECT_EQ(config.get<RectanglesVisibilityState>(),  true);
        valueChecker(config.get<VerticalLinePositions>(),  Positions{20, 50,125, 250, 500, 1000, 2000, 4000, 8000.00, 16000});
        valueChecker(config.get<FrequencyTextPositions>(),  Positions{20, 50,125, 250, 500, 1000, 2000, 4000, 8000.00, 16000});
    }
};

TEST_F(ConfigReaderTests, checkDefaultConfig)
{
    ConfigReader configReader(theme, testFolderName);

    const auto &config = configReader.getConfig();
    checkDefaultConfig(config);
}

TEST_F(ConfigReaderTests, configReaderTest)
{

    ConfigFileReader configFileReader(theme, "modifiedConfigTest");
    configFileReader.createDirIfNotExists();

    PythonDataSourceEnabled pythonDataSourceEnabled{true};
    DefaultFullscreenState defaultFullscreenState{false};
    DynamicMaxHoldAccelerationStateOfFalling dynamicMaxHoldAccelerationStateOfFalling{false};
    DynamicMaxHoldVisibilityState dynamicMaxHoldVisibilityState{false};
    AdvancedColorSettings advancedColorSettings{"some random string :D"};
    BackgroundColorSettings backgroundColorSettings{"some random string..."};
    MaximizedWindowSize maximizedWindowSize{{123,456}};
    NormalWindowSize normalWindowSize{{456,123}};
    NumberOfSamples numberOfSamples{3};
    SamplingRate samplingRate{16000};
    DesiredFrameRate desiredFrameRate{120};
    GapWidthInRelationToRectangleWidth gapWidthInRelationToRectangleWidth{0};
    NumberOfSignalsForAveraging numberOfSignalsForAveraging{3};
    NumberOfSignalsForMaxHold numberOfSignalsForMaxHold{1};
    MaxQueueSize maxQueueSize{10};
    AlphaFactor alphaFactor{0.3};
    OffsetFactor offsetFactor{0.6};
    DynamicMaxHoldRectangleHeightInPercentOfScreenSize dynamicMaxHoldRectangleHeightInPercentOfScreenSize{1.2};
    DynamicMaxHoldSpeedOfFalling dynamicMaxHoldSpeedOfFalling{100};
    DynamicMaxHoldSecondarySpeedOfFalling dynamicMaxHoldSecondarySpeedOfFalling{1001};
    ColorOfStaticLines colorOfStaticLines{{0.2,0.3,0.2}};
    HorizontalLinePositions horizontalLinePositions{{-1,-2,-3}};
    Freqs freqs{{100,101,102}};
    SignalWindow signalWindow{{1,2,3}};
    ColorsOfRectangle colorsOfRectangle{{{{0},{1,2,3}},{{1},{4,5,6}}}};
    ColorsOfDynamicMaxHoldRectangle colorsOfDynamicMaxHoldRectangle{{{{0},{2,3,4}},{{1},{5,6,7}}}};
    ColorsOfDynamicMaxHoldSecondaryRectangle colorsOfDynamicMaxHoldSecondaryRectangle{{{{0},{5,6,7}},{{1},{8,9,10}}}};
    LinesVisibilityState linesVisibilityState{true};
    RectanglesVisibilityState rectanglesVisibilityState{false};
    ColorOfDynamicMaxHoldLine colorOfDynamicMaxHoldLine{{0.1,0.2,0.3,0.4}};
    ColorOfDynamicMaxHoldSecondaryLine colorOfDynamicMaxHoldSecondaryLine{{0.5,0.6,0.7,0.8}};
    ColorOfStaticText colorOfStaticText{{0.8, 0.7, 0.6, 5}};
    VerticalLinePositions verticalLinePositions{{1001,2002,3003,4004,5005}};
    FrequencyTextPositions frequencyTextPositions{{5005, 4004,3003,2002,1001}};


    configFileReader.writeBoolToFile("PythonDataSourceEnabled", comment, pythonDataSourceEnabled.value);
    configFileReader.writeBoolToFile("DefaultFullscreenState", comment, defaultFullscreenState.value);
    configFileReader.writeBoolToFile("DynamicMaxHoldAccelerationStateOfFalling", comment, dynamicMaxHoldAccelerationStateOfFalling.value);
    configFileReader.writeBoolToFile("DynamicMaxHoldVisibilityState", comment, dynamicMaxHoldVisibilityState.value);
    configFileReader.writeBoolToFile("LinesVisibilityState", comment, linesVisibilityState.value);
    configFileReader.writeBoolToFile("RectanglesVisibilityState", comment, rectanglesVisibilityState.value);
    configFileReader.writeStringToFile("AdvancedColorSettings", comment, advancedColorSettings.value);
    configFileReader.writeStringToFile("BackgroundColorSettings", comment, backgroundColorSettings.value);
    configFileReader.writeVectorToCsv("MaximizedWindowSize", comment, std::vector<float>{(float)maximizedWindowSize.value.first, (float)maximizedWindowSize.value.second});
    configFileReader.writeVectorToCsv("NormalWindowSize", comment, std::vector<float>{(float)normalWindowSize.value.first, (float)normalWindowSize.value.second});
    configFileReader.writeVectorToCsv("NumberOfSamples", comment, {(float)numberOfSamples.value});
    configFileReader.writeVectorToCsv("SamplingRate", comment, {(float)samplingRate.value});
    configFileReader.writeVectorToCsv("DesiredFrameRate", comment, {(float)desiredFrameRate.value});
    configFileReader.writeVectorToCsv("GapWidthInRelationToRectangleWidth", comment, {gapWidthInRelationToRectangleWidth.value});
    configFileReader.writeVectorToCsv("NumberOfSignalsForAveraging", comment, {(float)numberOfSignalsForAveraging.value});
    configFileReader.writeVectorToCsv("NumberOfSignalsForMaxHold", comment, {(float)numberOfSignalsForMaxHold.value});
    configFileReader.writeVectorToCsv("MaxQueueSize", comment, {(float)maxQueueSize.value});
    configFileReader.writeVectorToCsv("AlphaFactor", comment, {alphaFactor.value});
    configFileReader.writeVectorToCsv("OffsetFactor", comment, {offsetFactor.value});
    configFileReader.writeVectorToCsv("DynamicMaxHoldRectangleHeightInPercentOfScreenSize", comment, {dynamicMaxHoldRectangleHeightInPercentOfScreenSize.value});
    configFileReader.writeVectorToCsv("DynamicMaxHoldSpeedOfFalling", comment, {dynamicMaxHoldSpeedOfFalling.value});
    configFileReader.writeVectorToCsv("DynamicMaxHoldSecondarySpeedOfFalling", comment, {dynamicMaxHoldSecondarySpeedOfFalling.value});
    configFileReader.writeVectorToCsv("ColorOfStaticLines", comment, colorOfStaticLines.value);
    configFileReader.writeVectorToCsv("HorizontalLinePositions", comment, horizontalLinePositions.value);
    configFileReader.writeVectorToCsv("Frequencies", comment, freqs.value);
    configFileReader.writeVectorToCsv("SignalWindow", comment, signalWindow.value);
    configFileReader.writeVectorToCsv("ColorOfDynamicMaxHoldLine", comment, colorOfDynamicMaxHoldLine.value);
    configFileReader.writeVectorToCsv("ColorOfDynamicMaxHoldSecondaryLine", comment, colorOfDynamicMaxHoldSecondaryLine.value);
    configFileReader.writeVectorToCsv("ColorOfStaticText", comment, colorOfStaticText.value);
    configFileReader.writeVectorToCsv("VerticalLinePositions", comment, verticalLinePositions.value);
    configFileReader.writeVectorToCsv("FrequencyTextPositions", comment, frequencyTextPositions.value);
    configFileReader.writeMapToCsv("ColorsOfRectangle", comment, colorsOfRectangle.value);
    configFileReader.writeMapToCsv("ColorsOfDynamicMaxHoldRectangle", comment, colorsOfDynamicMaxHoldRectangle.value);
    configFileReader.writeMapToCsv("ColorsOfDynamicMaxHoldSecondaryRectangle", comment, colorsOfDynamicMaxHoldSecondaryRectangle.value);


    ConfigReader configReader(theme, "modifiedConfigTest");
    const auto &config = configReader.getConfig();

    EXPECT_EQ(config.get<PythonDataSourceEnabled>(), pythonDataSourceEnabled.value);
    EXPECT_EQ(config.get<DefaultFullscreenState>(), defaultFullscreenState.value);
    EXPECT_EQ(config.get<DynamicMaxHoldAccelerationStateOfFalling>(), dynamicMaxHoldAccelerationStateOfFalling.value);
    EXPECT_EQ(config.get<DynamicMaxHoldVisibilityState>(), dynamicMaxHoldVisibilityState.value);
    EXPECT_EQ(config.get<LinesVisibilityState>(), linesVisibilityState.value);
    EXPECT_EQ(config.get<RectanglesVisibilityState>(), rectanglesVisibilityState.value);
    EXPECT_EQ(config.get<AdvancedColorSettings>(), advancedColorSettings.value);
    EXPECT_EQ(config.get<BackgroundColorSettings>(), backgroundColorSettings.value);
    EXPECT_EQ(config.get<MaximizedWindowSize>().first, maximizedWindowSize.value.first);
    EXPECT_EQ(config.get<MaximizedWindowSize>().second, maximizedWindowSize.value.second);
    EXPECT_EQ(config.get<NormalWindowSize>().first, normalWindowSize.value.first);
    EXPECT_EQ(config.get<NormalWindowSize>().second, normalWindowSize.value.second);
    EXPECT_EQ(config.get<NumberOfSamples>(), numberOfSamples.value);
    EXPECT_EQ(config.get<SamplingRate>(), samplingRate.value);
    EXPECT_EQ(config.get<DesiredFrameRate>(), desiredFrameRate.value);
    EXPECT_EQ(config.get<GapWidthInRelationToRectangleWidth>(), gapWidthInRelationToRectangleWidth.value);
    EXPECT_EQ(config.get<NumberOfSignalsForAveraging>(), numberOfSignalsForAveraging.value);
    EXPECT_EQ(config.get<NumberOfSignalsForMaxHold>(), numberOfSignalsForMaxHold.value);
    EXPECT_EQ(config.get<MaxQueueSize>(), maxQueueSize.value);
    EXPECT_NEAR(config.get<AlphaFactor>(), alphaFactor.value, precision);
    EXPECT_NEAR(config.get<OffsetFactor>(), offsetFactor.value, precision);
    EXPECT_NEAR(config.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>(), dynamicMaxHoldRectangleHeightInPercentOfScreenSize.value, precision);
    EXPECT_NEAR(config.get<DynamicMaxHoldSpeedOfFalling>(), dynamicMaxHoldSpeedOfFalling.value, lowPrecision);
    valueChecker(config.get<ColorOfStaticLines>(), colorOfStaticLines.value);
    valueChecker(config.get<HorizontalLinePositions>(),  horizontalLinePositions.value);
    valueChecker(config.get<Freqs>(), freqs.value);
    valueChecker(config.get<ColorOfDynamicMaxHoldLine>(), colorOfDynamicMaxHoldLine.value);
    valueChecker(config.get<ColorOfDynamicMaxHoldSecondaryLine>(), colorOfDynamicMaxHoldSecondaryLine.value);
    valueChecker(config.get<ColorOfStaticText>(), colorOfStaticText.value);
    valueChecker(config.get<VerticalLinePositions>(), verticalLinePositions.value);
    valueChecker(config.get<FrequencyTextPositions>(), frequencyTextPositions.value);
    EXPECT_EQ(config.get<SignalWindow>(), signalWindow.value);
    positionValuesChecker(config.get<ColorsOfRectangle>(), colorsOfRectangle.value);
    positionValuesChecker(config.get<ColorsOfDynamicMaxHoldRectangle>(), colorsOfDynamicMaxHoldRectangle.value);
    positionValuesChecker(config.get<ColorsOfDynamicMaxHoldSecondaryRectangle>(), colorsOfDynamicMaxHoldSecondaryRectangle.value);
}
