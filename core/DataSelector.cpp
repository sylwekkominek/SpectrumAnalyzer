/*
 * Copyright (C) 2024-2026, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "DataSelector.hpp"
#include "Helpers.hpp"

DataSelector::DataSelector(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies)
    : fftSize(fftSize), numberOfRectangles(demandedFrequencies.size()), selectedData(numberOfRectangles,0)
{

    uint32_t fftForRealDataSize=fftSize/2;

    for(uint32_t i=0;i<fftForRealDataSize; ++i)
    {
        allFrequencies.insert(samplingRate * ((float)i / fftSize));
    }

    updateIndexes(demandedFrequencies);

}

std::vector<float> DataSelector::operator()(const std::vector<float> &data)
{
    if(data.size() != fftSize)
    {
        throw std::runtime_error("Index selector error: not mached fftsize with signal length");
    }

    for(uint32_t i=0;i<numberOfRectangles;++i)
    {
        selectedData[i] = data.at(getFrequencyInfo(i).first);
    }

    return selectedData;
}

std::vector<float> DataSelector::operator()() const
{
    return selectedData;
}

Frequencies DataSelector::getSelectedFrequencies() const
{
    Frequencies selectedFrequencies;

    for(uint32_t i=0;i<numberOfRectangles;++i)
    {
        selectedFrequencies.push_back(getFrequencyInfo(i).second);
    }

    return selectedFrequencies;
}

std::vector<uint16_t> DataSelector::getRectangleIndexesClosestToFrequencies(const Frequencies &demandedFrequencies)
{
    std::vector<ClosestFrequency> closestFrequencies;
    std::vector<uint16_t> indexes;
    std::set<Frequency> rectanglesFrequencies;

    for(const auto &[rectangleIndex, frequencyInfo] : indexesMap)
    {
        rectanglesFrequencies.emplace(frequencyInfo.second);
    }

    updateContainer(rectanglesFrequencies, demandedFrequencies, [&](const RectangleIndex &, const FrequencyInfo &frequencyInfo){
        closestFrequencies.push_back(frequencyInfo.second);
    });

    for(const auto &closestFrequency : closestFrequencies)
    {
        for(auto &[rectangleIndex, frequencyInfo] : indexesMap)
        {
            if(isEqual<float>(frequencyInfo.second, closestFrequency))
            {
                indexes.push_back(rectangleIndex);
            }
        }
    }

    return indexes;
}

void DataSelector::updateContainer(std::set<Frequency> availableFrequencies, const Frequencies &demandedFrequencies, std::function<void(const RectangleIndex &, const FrequencyInfo &)> function)
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
    }
}

void DataSelector::updateIndexes(const Frequencies &demandedFrequencies)
{
    updateContainer(allFrequencies, demandedFrequencies, [this](const RectangleIndex &rectangleIndex, const FrequencyInfo &frequencyInfo){
        indexesMap.insert({rectangleIndex,frequencyInfo});
    });
}

DataSelector::FrequencyInfo DataSelector::getFrequencyInfo(RectangleIndex rectangleNumber) const
{
    return indexesMap.at(rectangleNumber);
}
