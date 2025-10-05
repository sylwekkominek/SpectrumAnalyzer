#pragma once

#include <vector>
#include <cstdint>

class SamplesCollectorBase
{
public:
    SamplesCollectorBase()=default;
    virtual ~SamplesCollectorBase()=default;
    bool virtual initialize(uint32_t numberOfSamples, uint32_t samplingRate)=0;
    std::vector<float> virtual collectDataFromHw()=0;

protected:
    uint8_t static constexpr numberOfChannels{2};
    uint32_t dataLength{0};
};


