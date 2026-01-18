/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once
#include "DataCalculator.hpp"
#include <cstdint>
#include <any>
#include <typeindex>
#include <map>
#include <optional>
#include <stdexcept>

struct CursorPosition
{
    double x;
    double y;
};

struct WindowSize
{
    uint16_t x;
    uint16_t y;
};


class AnyData
{
public:
    template <typename T>
    void add(T&& t)
    {
        data[typeid(T)] = std::any(std::forward<T>(t));
    }

    template <typename T>
    T& get()
    {
        try
        {
            return std::any_cast<T&>(data.at(typeid(T)));
        }
        catch (const std::out_of_range&)
        {
            throw std::runtime_error(std::string("AnyData: missing type: ") + typeid(T).name());
        }
    }

    template <typename T>
    const T& get() const
    {
        try
        {
            return std::any_cast<const T&>(data.at(typeid(T)));
        }
        catch (const std::out_of_range&)
        {
            throw std::runtime_error(std::string("AnyData: missing type: ") + typeid(T).name());
        }
    }


    bool empty() const
    {
        return data.empty();
    }

private:
    std::map<std::type_index, std::any> data;
};


class Averager
{
public:

    Averager(uint32_t numberOfValues): dataAverager(1, numberOfValues)
    {
    }

    std::optional<float> operator()(const float value)
    {
        dataAverager.push_back({value});

        auto averagedData = dataAverager.calculate();

        if(not averagedData.empty())
        {
            return averagedData.front();
        }
        return std::nullopt;
    }

private:
    DataAverager dataAverager;
};
