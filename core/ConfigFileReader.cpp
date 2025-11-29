/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "ConfigFileReader.hpp"
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>

ConfigFileReader::ConfigFileReader(const std::string &path) : path(path)
{
}

std::string ConfigFileReader::getPathWithFileName(const std::string &fileName)
{
    std::filesystem::path fullPath = path;
    fullPath /= fileName;
    fullPath += ".txt";
    return std::filesystem::absolute(fullPath).string();
}

std::ofstream ConfigFileReader::createOutputFileStreamWithComment(const std::string& fileName, const std::string& comment)
{
    const auto path = getPathWithFileName(fileName);

    std::ofstream file(path);

    if (!file)
    {
        throw std::runtime_error("Cannot create file:  " + path);
    }

    if (!comment.empty())
    {
        file << comment << std::endl;
    }

    return file;
}

std::vector<std::string> ConfigFileReader::getFilteredDataLines(const std::string& fileName)
{
    const auto path = getPathWithFileName(fileName);

    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::vector<std::string> result;
    std::string line;

    while (std::getline(file, line))
    {
        const auto onlyWhitespaces = std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
        });

        if (onlyWhitespaces)
        {
            continue;
        }

        if (line.find("//", 0) == 0)
        {
            continue;
        }

        result.push_back(line);
    }

    if (result.empty())
    {
        throw std::runtime_error("No data in file: " + fileName);
    }

    return result;
}

void ConfigFileReader::createDirIfNotExists()
{
    std::filesystem::create_directories(path);
}

bool ConfigFileReader::checkIfFileExists(const std::string& fileName)
{
    return std::filesystem::exists(getPathWithFileName(fileName));
}

void ConfigFileReader::writeStringToFile(const std::string& fileName, const std::string &comment, const std::string& content)
{
    auto outputFile = createOutputFileStreamWithComment(fileName, comment);
    outputFile << content;
}

void ConfigFileReader::writeVectorToCsv(const std::string& fileName, const std::string &comment, const std::vector<float>& values, const uint8_t precision)
{
    auto outputFile = createOutputFileStreamWithComment(fileName, comment);

    outputFile << std::fixed << std::setprecision(precision);

    for (size_t i = 0; i < values.size(); ++i)
    {
        outputFile << values[i];

        if (i != values.size() - 1)
        {
            outputFile << ", ";
        }

    }
}

void ConfigFileReader::writeMapToCsv(const std::string& fileName, const std::string &comment, const std::map<uint32_t, std::vector<float>>& data, const uint8_t precision)
{
    auto outputFile = createOutputFileStreamWithComment(fileName, comment);

    outputFile << std::fixed << std::setprecision(precision);

    for (const auto& [key, vec] : data)
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            outputFile << vec[i];

            if (i != vec.size() - 1)
            {
                outputFile << ", ";
            }
        }
        outputFile << "\n";
    }
}

void ConfigFileReader::writeBoolToFile(const std::string& fileName, const std::string &comment,  bool value)
{
    auto outputFile = createOutputFileStreamWithComment(fileName, comment);
    outputFile << (value ? "true" : "false") << "\n";
}

std::string ConfigFileReader::readWholeFile(const std::string& filename)
{
    const auto lines = getFilteredDataLines(filename);

    std::ostringstream oss;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        oss << lines[i];

        if (i != lines.size() - 1)
        {
            oss <<'\n';
        }
    }

    return oss.str();
}

std::vector<float> ConfigFileReader::readCSVToVector(const std::string& fileName)
{
    auto lines = getFilteredDataLines(fileName);
    auto &line = lines.at(0);

    for (auto& c : line)
    {
        if (c == ',' || c == ';')
            c = ' ';
    }

    std::istringstream iss(line);
    std::istream_iterator<float> begin(iss);
    std::istream_iterator<float> end;

    std::vector<float> values(begin, end);
    return values;
}

std::map<uint32_t, std::vector<float>> ConfigFileReader::readCsvToMap(const std::string& fileName)
{
    auto lines = getFilteredDataLines(fileName);

    std::map<uint32_t, std::vector<float>> result;
    uint32_t rowIndex = 0;

    for(auto &line  : lines)
    {
        for (auto& c : line)
        {
            if (c == ',' || c == ';')
                c = ' ';
        }

        std::istringstream iss(line);
        std::istream_iterator<float> begin(iss);
        std::istream_iterator<float> end;

        std::vector<float> row(begin, end);
        if (!row.empty())
        {
            result[rowIndex++] = std::move(row);
        }
    }

    return result;
}

bool ConfigFileReader::readBoolFromFile(const std::string& fileName)
{
    auto lines = getFilteredDataLines(fileName);
    auto line = lines.at(0);

    std::transform(line.begin(), line.end(), line.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (line == "true")
    {
        return true;
    }

    if (line == "false")
    {
        return false;
    }

    try
    {
        const float epsilon{0.01};
        const double val = std::stod(line);

        if (std::abs(val) < epsilon)
        {
            return false;
        }
    }
    catch(...)
    {
        throw std::runtime_error("Wrong data type in file: " + fileName);
    }

    return true;
}

