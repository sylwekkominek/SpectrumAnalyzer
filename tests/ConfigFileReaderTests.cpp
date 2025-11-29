/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/ConfigFileReader.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class ConfigFileReaderTests : public ValuesChecker<-6>, public ::testing::Test
{
public:
    const std::string testFolderName = "configFileReaderTests";
};

TEST_F(ConfigFileReaderTests, wholeFileTest)
{
    ConfigFileReader configFileReader(testFolderName);

    const std::string fileName("wholeFileTest");
    const std::string comment(R"(//This is comment)");
    const std::string content("This is text");

    configFileReader.createDirIfNotExists();
    configFileReader.writeStringToFile(fileName, comment, content);

    EXPECT_EQ(content, configFileReader.readWholeFile(fileName));
}

TEST_F(ConfigFileReaderTests, vectorToCsvTest)
{
    ConfigFileReader configFileReader(testFolderName);

    const std::string fileName("vectorToCsvTest");
    const std::string comment(R"(//This is comment)");
    const std::vector<float> values({-3.0003, -2.0002, -1.0001, 0.0, 1.0001, 2.0002, 3.0003});

    configFileReader.createDirIfNotExists();
    configFileReader.writeVectorToCsv(fileName, comment, values,4);

    valueChecker(values, configFileReader.readCSVToVector(fileName));
}

TEST_F(ConfigFileReaderTests, mapToCsvTest)
{
    ConfigFileReader configFileReader(testFolderName);

    const std::string fileName("mapToCsvTest");
    const std::string comment(R"(//This is comment)");
    const std::map<uint32_t, std::vector<float>> values = {
        { 0, { -2.0002f, -1.0001f,  0.0f,  1.0001f,  2.0002f } },
        { 1, { -1.0001f,  0.0f,  1.0001f,  2.0002f,  3.0003f } },
        { 2, {  0.0f,  1.0001f,  2.0002f,  3.0003f,  4.0004f } },
        { 3, {  1.0001f,  2.0002f,  3.0003f,  4.0004f,  5.0005f } },
        { 4, {  2.0002f,  3.0003f,  4.0004f,  5.0005f,  6.0006f } }
    };

    configFileReader.createDirIfNotExists();
    configFileReader.writeMapToCsv(fileName, comment, values,4);
    const auto result = configFileReader.readCsvToMap(fileName);

    positionValuesChecker(values, result);
}

TEST_F(ConfigFileReaderTests, boolToFileTest)
{
    ConfigFileReader configFileReader(testFolderName);

    const std::string fileName("boolToFileTest");
    const std::string comment(R"(//This is comment)");
    const bool negativeValue = false;
    const bool positiveValue = true;

    configFileReader.writeBoolToFile(fileName, comment, negativeValue);
    EXPECT_EQ(negativeValue, configFileReader.readBoolFromFile(fileName));

    configFileReader.writeBoolToFile(fileName, comment, positiveValue);
    EXPECT_EQ(positiveValue, configFileReader.readBoolFromFile(fileName));
}
