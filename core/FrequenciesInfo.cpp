/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "FrequenciesInfo.hpp"
#include "Helpers.hpp"
#include <algorithm>

FrequenciesInfo::FrequenciesInfo(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies)
    : fftSize(fftSize), numberOfRectangles(demandedFrequencies.size()), selectedData(numberOfRectangles,0)
{
    updateAllFrequencies(samplingRate, fftSize);
    updateClosestFrequenciesMap(demandedFrequencies);
    updateAllFrequenciesMap(demandedFrequencies);
}

uint32_t FrequenciesInfo::numberOfFrequencies()
{
    return numberOfRectangles;
}

Frequencies FrequenciesInfo::getAllFrequencies() const
{
    return Frequencies(allFrequencies.begin(), allFrequencies.end());
}

std::vector<RectangleIndex> FrequenciesInfo::getRectangleIndexesClosestToFrequencies(const Frequencies &demandedFrequencies)
{
    std::vector<ClosestFrequency> closestFrequencies;
    std::vector<RectangleIndex> indexes;
    std::set<Frequency> rectanglesFrequencies;

    for(const auto &[rectangleIndex, frequencyInfo] : closestFrequenciesMap)
    {
        rectanglesFrequencies.emplace(frequencyInfo.second);
    }

    updateContainer(rectanglesFrequencies, demandedFrequencies, [&](const RectangleIndex &, const Info &frequencyInfo){
        closestFrequencies.push_back(frequencyInfo.second);
    });

    for (auto &closestFrequency : closestFrequencies)
    {
        auto it = std::find_if(closestFrequenciesMap.begin(),closestFrequenciesMap.end(),[&](const auto& pair)
            {
                    return isEqual<float>(pair.second.second, closestFrequency);
            });

        if (it != closestFrequenciesMap.end())
        {
            indexes.push_back(it->first);
        }
    }

    return indexes;
}

void FrequenciesInfo::updateContainer(std::set<Frequency> availableFrequencies, const Frequencies &demandedFrequencies, std::function<void(const RectangleIndex &, const Info &)> function)
{
    for(uint32_t i=0;i<demandedFrequencies.size();++i)
    {
        auto demandedFreq = demandedFrequencies.at(i);

        auto upperFreq = availableFrequencies.upper_bound(demandedFreq);
        auto lowerFreq = upperFreq;


        if((upperFreq != availableFrequencies.end()) and (upperFreq != availableFrequencies.begin()))
        {
            --lowerFreq;

            const auto shortestDistanceIterator = (demandedFreq- *lowerFreq) <= (*upperFreq-demandedFreq) ? lowerFreq : upperFreq;
            const auto index = std::distance(availableFrequencies.begin(),shortestDistanceIterator);

            function(i,std::make_pair(index, *shortestDistanceIterator));
        }
        else if( upperFreq == availableFrequencies.begin())
        {
            function(i,std::make_pair(0, *availableFrequencies.begin()));
        }
        else if( upperFreq == availableFrequencies.end())
        {
            function(i,std::make_pair(i, *availableFrequencies.rbegin()));
        }
    }
}

void FrequenciesInfo::updateAllFrequencies(uint32_t samplingRate, uint32_t fftSize)
{
    uint32_t fftForRealDataSize=fftSize/2;

    for(uint32_t i=0;i<fftForRealDataSize; ++i)
    {
        allFrequencies.insert(samplingRate * ((float)i / fftSize));
    }
}

void FrequenciesInfo::updateClosestFrequenciesMap(const Frequencies &demandedFrequencies)
{
    updateContainer(allFrequencies, demandedFrequencies, [this](const RectangleIndex &rectangleIndex, const Info &frequencyInfo){
        closestFrequenciesMap.insert({rectangleIndex,frequencyInfo});
    });
}

void FrequenciesInfo::updateAllFrequenciesMap(const Frequencies &demandedFrequencies)
{

    std::set<Frequency> frequencies;

    const float bandStart = 1; //just to avoid constant member
    const float bandEnd = 20100; //just to avoid out-of-band signals

    const float firstHalfStep = (demandedFrequencies[1] - demandedFrequencies[0]) * 0.5f;
    const float lastHalfStep  = (demandedFrequencies[demandedFrequencies.size() - 1] - demandedFrequencies[demandedFrequencies.size() - 2]) * 0.5f;

    frequencies.emplace(std::max(bandStart, demandedFrequencies.front() - firstHalfStep));
    frequencies.emplace(std::min(bandEnd, demandedFrequencies.back()  + lastHalfStep));


    for (uint32_t i = 1; i < demandedFrequencies.size()-1; ++i)
    {
        float fLow = (demandedFrequencies[i-1] + demandedFrequencies[i])/2;
        float fHigh = (demandedFrequencies[i] + demandedFrequencies[i+1])/2;

        if((fLow >= bandStart) && (fHigh<=bandEnd))
        {
            frequencies.emplace(fLow);
            frequencies.emplace(fHigh);
        }
    }

    Frequencies frequenciesVec(frequencies.begin(), frequencies.end());
    Frequencies allFrequencies = getAllFrequencies();

    for (RectangleIndex rectangleIndex = 0; rectangleIndex < frequenciesVec.size()-1; ++rectangleIndex)
    {
        float fLow = frequenciesVec[rectangleIndex];
        float fHigh = frequenciesVec[rectangleIndex+1];

        std::vector<Info> values;

        for (FrequencyIndex frequencyIndex = 0; frequencyIndex < allFrequencies.size(); ++frequencyIndex)
        {
            float fftFrequency = allFrequencies.at(frequencyIndex);

            if (fftFrequency >= fLow && fftFrequency < fHigh)
            {
                values.push_back(std::make_pair(frequencyIndex, fftFrequency));
            }
        }

        allFrequenciesMap.emplace(rectangleIndex, std::move(values));
    }
}

FrequenciesInfo::Info FrequenciesInfo::getFrequencyInfo(RectangleIndex rectangleNumber) const
{
    return closestFrequenciesMap.at(rectangleNumber);
}

FrequencyIndexesPerRectangle FrequenciesInfo::getAllFrequencyIndexes()
{
    FrequencyIndexesPerRectangle frequencyIndexesPerRectangle;

    for(const auto &[rectangleIndex, frequencyInfos] : allFrequenciesMap)
    {
        std::vector<FrequencyIndex>  frequencyIndexes;
        frequencyIndexes.reserve(frequencyInfos.size());

        for (const auto& [frequencyIndex, frequency] : frequencyInfos)
        {
            frequencyIndexes.push_back(frequencyIndex);
        }

        frequencyIndexesPerRectangle.emplace(rectangleIndex, std::move(frequencyIndexes));
    }

    return frequencyIndexesPerRectangle;
}

std::vector<FrequencyRange> FrequenciesInfo::getFrequencyRangeForEachRectangle()
{
    std::vector<FrequencyRange> frequencyRangePerRectangle;

    for(const auto &[rectangleIndex, frequencyInfos] : allFrequenciesMap)
    {
        FrequencyRange frequencyRange{};

        if(!frequencyInfos.empty())
        {
            frequencyRange.first = frequencyInfos.front().second;
            frequencyRange.second = frequencyInfos.back().second;
        }

        frequencyRangePerRectangle.emplace_back(frequencyRange);
    }

    return frequencyRangePerRectangle;
}




