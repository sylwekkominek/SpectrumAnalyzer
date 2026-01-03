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
        EXPECT_EQ(config.get<NumberOfSamples>(), 4096);
        EXPECT_EQ(config.get<SamplingRate>(), 44100);
        EXPECT_EQ(config.get<DesiredFrameRate>(), 60);
        EXPECT_EQ(config.get<GapWidthInRelationToRectangleWidth>(), 0);
        EXPECT_EQ(config.get<NumberOfSignalsForAveraging>(), 1);
        EXPECT_EQ(config.get<NumberOfSignalsForMaxHold>(), 5);
        EXPECT_EQ(config.get<MaxQueueSize>(), 20);
        EXPECT_NEAR(config.get<AlphaFactor>(), 0.25, precision);
        EXPECT_NEAR(config.get<ScalingFactor>(), 2.000488, precision);
        EXPECT_NEAR(config.get<OffsetFactor>(), 0, precision);
        EXPECT_NEAR(config.get<DynamicMaxHoldRectangleHeightInPercentOfScreenSize>(), 0.8, precision);
        EXPECT_NEAR(config.get<DynamicMaxHoldSpeedOfFalling>(), 900, lowPrecision);
        EXPECT_NEAR(config.get<DynamicMaxHoldTransparentSpeedOfFalling>(), 1000, lowPrecision);
        EXPECT_EQ(config.get<DynamicMaxHoldAccelerationStateOfFalling>(), true);
        EXPECT_EQ(config.get<DynamicMaxHoldVisibilityState>(), true);
        EXPECT_EQ(config.get<AdvancedColorSettings>().size(), 6739);
        EXPECT_EQ(config.get<BackgroundColorSettings>().size(), 4872);
        valueChecker(config.get<ColorOfStaticLines>(),  Color{0.15,0.15,0.15,1});
        valueChecker(config.get<HorizontalLinePositions>(),  Positions{-6.02, -12.04, -18.06, -24.08, -30.10, -36.12, -36.12, -42.14, -48.16, -54.18, -60.20, -66.22, -72.24, -78.26, -84.28, -90.30});
        valueChecker(config.get<Freqs>(), Frequencies{20,40,60,80,100,120,150,180,220,250,300,330,360,400,440, 480,520,560,600,720,840,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900, 2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,4100,4200,4300,4400,4500,4600,4700,4800,4900,5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,6000,6100,6200,6300,6400,6500,6600,6700,6800,6900,7000,7100,7200,7300,7400,7500,7600,7700,7800,7900,8000});
        EXPECT_EQ(config.get<SignalWindow>().size(), 4096);
        positionValuesChecker(colors, config.get<ColorsOfRectangle>());
        positionValuesChecker(colors, config.get<ColorsOfDynamicMaxHoldRectangle>());
        positionValuesChecker(transparentColors, config.get<ColorsOfDynamicMaxHoldTransparentRectangle>());

    }
};

TEST_F(ConfigReaderTests, checkDefaultConfig)
{
    ConfigReader configReader(testFolderName);

    const auto &config = configReader.getConfig();
    checkDefaultConfig(config);
}

TEST_F(ConfigReaderTests, configReaderTest)
{

    ConfigFileReader configFileReader("modifiedConfigTest");
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
    DynamicMaxHoldTransparentSpeedOfFalling dynamicMaxHoldTransparentSpeedOfFalling{1001};
    ColorOfStaticLines colorOfStaticLines{{0.2,0.3,0.2}};
    HorizontalLinePositions horizontalLinePositions{{-1,-2,-3}};
    Freqs freqs{{100,101,102}};
    SignalWindow signalWindow{{1,2,3}};
    ColorsOfRectangle colorsOfRectangle{{{{0},{1,2,3}},{{1},{4,5,6}}}};
    ColorsOfDynamicMaxHoldRectangle colorsOfDynamicMaxHoldRectangle{{{{0},{2,3,4}},{{1},{5,6,7}}}};
    ColorsOfDynamicMaxHoldTransparentRectangle colorsOfDynamicMaxHoldTransparentRectangle{{{{0},{5,6,7}},{{1},{8,9,10}}}};

    configFileReader.writeBoolToFile("PythonDataSourceEnabled", comment, pythonDataSourceEnabled.value);
    configFileReader.writeBoolToFile("DefaultFullscreenState", comment, defaultFullscreenState.value);
    configFileReader.writeBoolToFile("DynamicMaxHoldAccelerationStateOfFalling", comment, dynamicMaxHoldAccelerationStateOfFalling.value);
    configFileReader.writeBoolToFile("DynamicMaxHoldVisibilityState", comment, dynamicMaxHoldVisibilityState.value);
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
    configFileReader.writeVectorToCsv("DynamicMaxHoldTransparentSpeedOfFalling", comment, {dynamicMaxHoldTransparentSpeedOfFalling.value});
    configFileReader.writeVectorToCsv("ColorOfStaticLines", comment, colorOfStaticLines.value);
    configFileReader.writeVectorToCsv("HorizontalLinePositions", comment, horizontalLinePositions.value);
    configFileReader.writeVectorToCsv("Frequencies", comment, freqs.value);
    configFileReader.writeVectorToCsv("SignalWindow", comment, signalWindow.value);
    configFileReader.writeMapToCsv("ColorsOfRectangle", comment, colorsOfRectangle.value);
    configFileReader.writeMapToCsv("ColorsOfDynamicMaxHoldRectangle", comment, colorsOfDynamicMaxHoldRectangle.value);
    configFileReader.writeMapToCsv("ColorsOfDynamicMaxHoldTransparentRectangle", comment, colorsOfDynamicMaxHoldTransparentRectangle.value);


    ConfigReader configReader("modifiedConfigTest");
    const auto &config = configReader.getConfig();

    EXPECT_EQ(config.get<PythonDataSourceEnabled>(), pythonDataSourceEnabled.value);
    EXPECT_EQ(config.get<DefaultFullscreenState>(), defaultFullscreenState.value);
    EXPECT_EQ(config.get<DynamicMaxHoldAccelerationStateOfFalling>(), dynamicMaxHoldAccelerationStateOfFalling.value);
    EXPECT_EQ(config.get<DynamicMaxHoldVisibilityState>(), dynamicMaxHoldVisibilityState.value);
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
    EXPECT_EQ(config.get<SignalWindow>(), signalWindow.value);
    positionValuesChecker(config.get<ColorsOfRectangle>(), colorsOfRectangle.value);
    positionValuesChecker(config.get<ColorsOfDynamicMaxHoldRectangle>(), colorsOfDynamicMaxHoldRectangle.value);
    positionValuesChecker(config.get<ColorsOfDynamicMaxHoldTransparentRectangle>(), colorsOfDynamicMaxHoldTransparentRectangle.value);
}
