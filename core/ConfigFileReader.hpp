/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

class ConfigFileReader
{
public:
    ConfigFileReader(const std::string &path="config");

    void writeStringToFile(const std::string& fileName, const std::string &comment, const std::string& content);
    void writeVectorToCsv(const std::string& fileName, const std::string &comment, const std::vector<float>& values, const uint8_t precision=8);
    void writeMapToCsv(const std::string& fileName, const std::string &comment, const std::map<uint32_t, std::vector<float>>& data, const uint8_t precision=8);
    void writeBoolToFile(const std::string& fileName, const std::string &comment,  bool value = false);

    std::string readWholeFile(const std::string& filename);
    std::vector<float> readCSVToVector(const std::string& fileName);
    std::map<uint32_t, std::vector<float>> readCsvToMap(const std::string& fileName);
    bool readBoolFromFile(const std::string& fileName);
    void createDirIfNotExists();
    std::string getConfigPath();

protected:

    bool checkIfFileExists(const std::string& fileName);
    std::vector<std::string> getFilteredDataLines(const std::string& fileName);
    std::ofstream createOutputFileStreamWithComment(const std::string& fileName, const std::string& comment = "");
    std::string getPathWithFileName(const std::string &fileName);

    const std::string path;
};
