/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "IndexSelector.hpp"
#include <iostream>


IndexSelector::IndexSelector(uint32_t samplingRate, uint32_t fftSize, const Frequencies &demandedFrequencies)
    : numberOfRectangles(demandedFrequencies.size())
{

    uint32_t fftForRealDataSize=fftSize/2;

    for(uint i=0;i<fftForRealDataSize; ++i)
    {
        allFrequencies.insert(samplingRate * ((float)i / fftSize));
    }

    updateIndexes(demandedFrequencies);
}

void IndexSelector::updateIndexes(const Frequencies &demandedFrequencies)
{

    for(uint i=0;i<numberOfRectangles;++i)
    {
        auto demandedFreq = demandedFrequencies.at(i);

        auto upperFreq = allFrequencies.upper_bound(demandedFreq);
        auto lowerFreq = upperFreq;

        if((upperFreq != allFrequencies.end()) and (upperFreq != allFrequencies.begin()))
        {
           --lowerFreq;

            const auto shortestDistanceIterator = (demandedFreq- *lowerFreq) <= (*upperFreq-demandedFreq) ? lowerFreq : upperFreq;
            const auto index = std::distance(allFrequencies.begin(),shortestDistanceIterator);

            std::cout<<"lowerFreq: "<< *lowerFreq <<" demanded freq: "<< demandedFreq <<"  upperFreq: "<<*upperFreq<<" used: "<<*shortestDistanceIterator<<" index: "<<index<<std::endl;

            indexesMap.insert({i,index});
        }
        else if( upperFreq == allFrequencies.begin())
        {
            indexesMap.insert({i,0});
        }
    }
}

uint32_t IndexSelector::getFrequencyIndex(uint32_t frequencyNumber) const
{
    return indexesMap.at(frequencyNumber);
}



