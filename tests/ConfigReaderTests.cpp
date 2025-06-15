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
    const char *pythonFileName = "testConfig";
};

TEST_F(ConfigReaderTests, configReaderTest)
{
    ConfigReader configReader(pythonFileName);

    const auto &config = configReader.getConfig();

    EXPECT_EQ(config.horizontalSize, 1);
    EXPECT_EQ(config.verticalSize, 2);
    EXPECT_EQ(config.numberOfSamples, 3);
    EXPECT_EQ(config.samplingRate, 4);
    EXPECT_EQ(config.desiredFrameRate, 5);
    EXPECT_NEAR(config.gapWidthInRelationToRectangleWidth, 5.1, marginOfError);
    EXPECT_EQ(config.numberOfSignalsForAveraging, 6);
    EXPECT_EQ(config.numberOfSignalsForMaxHold, 7);
    EXPECT_EQ(config.maxQueueSize, 8);
    EXPECT_NEAR(config.alphaFactor, 8.1, marginOfError);
    EXPECT_EQ(config.scalingFactor, 9);
    EXPECT_EQ(config.offsetFactor, 10);
    EXPECT_NEAR(config.dynamicMaxHoldRectangleHeightInPercentOfScreenSize, 10.1, marginOfError);
    EXPECT_EQ(config.dynamicMaxHoldSpeedOfFalling, 11);
    EXPECT_EQ(config.dynamicMaxHoldVisibilityState, true);
    EXPECT_EQ(config.advancedColorSettings, std::string("test"));

    valueChecker({1,2,3}, config.frequencies);
    valueChecker({4,5,6}, config.signalWindow);

    valueChecker({1.1, 1.2, 1.3}, config.colorsOfRectangle.at(0));
    valueChecker({2.1, 2.2, 2.3}, config.colorsOfRectangle.at(1));
    valueChecker({3.1, 3.2, 3.3}, config.colorsOfRectangle.at(2));
    valueChecker({4.1, 4.2, 4.3}, config.colorsOfRectangle.at(3));

    valueChecker({1.1, 2.1, 3.1}, config.colorsOfDynamicMaxHoldRectangle.at(0));
    valueChecker({1.2, 2.2, 3.2}, config.colorsOfDynamicMaxHoldRectangle.at(1));
    valueChecker({1.3, 2.3, 3.3}, config.colorsOfDynamicMaxHoldRectangle.at(2));
    valueChecker({1.4, 2.4, 3.4}, config.colorsOfDynamicMaxHoldRectangle.at(3));
}
