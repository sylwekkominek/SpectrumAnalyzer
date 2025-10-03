/*
 * Copyright (C) 2024-2025, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#pragma once

#include <algorithm>
#include <deque>
#include <functional>
#include <cstdint>

template<typename T>
class DataCalculatorBase
{
public:

    DataCalculatorBase(uint32_t numberOfSamples, uint32_t numberOfSignalsForProcessing, float initValue=0);
    virtual std::vector<T> calculate() =0;

    void push_back(const std::vector<T>&data);
    void clear();

protected:


    virtual void updateAlgorithm()=0;

    std::vector<T> calculateWithMoving();
    std::vector<T> calculateWithClearing();
    std::vector<T> calculateWithStoring();

    std::function<T(T&,T&)> algorithm;
    uint32_t numberOfSignalsForProcessing;

private:

    std::deque<std::vector<T>> dataQueue;
    std::vector<T> data;
    float initValue;
    uint32_t numberOfSamples;
};


template<typename T>
DataCalculatorBase<T>::DataCalculatorBase(uint32_t numberOfSamples, uint32_t numberOfSignalsForProcessing, float initValue):
    numberOfSamples(numberOfSamples),
    numberOfSignalsForProcessing(numberOfSignalsForProcessing), initValue(initValue), data(numberOfSamples,0)
{
}

template<typename T>
void DataCalculatorBase<T>::push_back(const std::vector<T>&data)
{
    dataQueue.push_back(data);
}

template<typename T>
std::vector<T> DataCalculatorBase<T>::calculateWithMoving()
{

    if(dataQueue.size() >= numberOfSignalsForProcessing)
    {
        data = std::vector<T>(numberOfSamples,initValue);

        for(uint32_t i =0;i<numberOfSignalsForProcessing;++i)
        {
            std::transform(data.begin(), data.end(), dataQueue[i].begin(), data.begin(), algorithm);
        }

        dataQueue.pop_front();
        return data;
    }
    return {};
}

template<typename T>
std::vector<T> DataCalculatorBase<T>::calculateWithClearing()
{

    if(dataQueue.size() >= numberOfSignalsForProcessing)
    {
        data = std::vector<T>(numberOfSamples,initValue);

        for(uint32_t i =0;i<numberOfSignalsForProcessing;++i)
        {
            std::transform(data.begin(), data.end(), dataQueue[i].begin(), data.begin(), algorithm);
        }

        dataQueue.clear();
        return data;
    }
    return {};
}


template<typename T>
std::vector<T> DataCalculatorBase<T>::calculateWithStoring()
{

    if(dataQueue.size() >= numberOfSignalsForProcessing)
    {
        for(uint32_t i =0;i<numberOfSignalsForProcessing;++i)
        {
            std::transform(data.begin(), data.end(), dataQueue[i].begin(), data.begin(), algorithm);
        }

        dataQueue.pop_front();
        return data;
    }
    return {};
}

template<typename T>
void DataCalculatorBase<T>::clear()
{
    dataQueue.clear();
}


class DataAverager : public DataCalculatorBase<float>
{
public:
    DataAverager(uint32_t numberOfSamples, uint32_t numberOfSignalsForProcessing);
    std::vector<float> calculate() override;
private:
    void updateAlgorithm() override;
};

class DataMaxHolder : public DataCalculatorBase<float>
{
public:
    DataMaxHolder(uint32_t numberOfSamples, uint32_t numberOfSignalsForProcessing, float initValue=0);
    std::vector<float> calculate() override;

private:
    void updateAlgorithm() override;
};

class DataSmoother : public DataCalculatorBase<float>
{
public:
    DataSmoother(uint32_t numberOfSamples, float alphaFactor);
    std::vector<float> calculate() override;

private:
    void updateAlgorithm() override;
    const float alphaFactor;
};

