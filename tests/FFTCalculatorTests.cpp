/*
 * Copyright (C) 2024, Sylwester Kominek
 * This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
 * see file LICENSE in this source tree.
 */

#include "core/FFTCalculator.hpp"
#include "helpers/TestHelpers.hpp"
#include "helpers/ValuesChecker.hpp"
#include <gtest/gtest.h>
#include <cmath>


class FFTCalculatorTestBase : public ValuesChecker<-4>, public ::testing::TestWithParam<FftType>
{
public:

    void verifyFftData(const FFTResult &dataInQueue, const std::map<Position,ExpectedValue> &expectedFftValues, const std::map<Position,ExpectedValue> &expectedFftValuesAfterNormalization, const std::map<Position,ExpectedValue> &expectedPhaseValues)
    {
        const auto absSignal = calculateAbs(dataInQueue);
        const auto phaseSignal = calculatePhase(dataInQueue);
        const auto normalizedData = normalizeComplexFFt(absSignal);

        positionValueChecker(expectedFftValues, absSignal);
        positionValueChecker(expectedFftValuesAfterNormalization, normalizedData);
        positionValueChecker(expectedPhaseValues, phaseSignal);
    }

    std::vector<float> normalizeComplexFFt(const std::vector<float> &fftData)
    {
        const uint32_t numberOfSamples = fftData.size();
        std::vector<float> outputData(numberOfSamples);

        std::transform(fftData.begin(), fftData.end(), outputData.begin(), [&numberOfSamples](const auto &el){ return (el/(numberOfSamples/2)); });
        return outputData;
    }
};

class FFTCalculatorTest : public FFTCalculatorTestBase
{
public:
    const uint32_t numberOfSamples{8};
    const uint32_t samplingFrequency{8000};

    std::vector<float> generateTestSignal(uint32_t signalFreqency, float amplitude, uint32_t phaseOffset=0)
    {
        return generateSignal(numberOfSamples,samplingFrequency,signalFreqency, amplitude, phaseOffset);
    }

    std::unique_ptr<FFTCalculatorBase> getCalculator(FftType fftType, uint32_t fftSize)
    {
        return (fftType == FftType::Complex)
                            ? std::unique_ptr<FFTCalculatorBase>(std::make_unique<ComplexFFTCalculator>(fftSize))
                            : std::unique_ptr<FFTCalculatorBase>(std::make_unique<RealFFTCalculator>(fftSize));
    }
};

TEST_F(FFTCalculatorTest, checkingRealAndComplexFft)
{
    const uint32_t signalFreqency{1234};
    const float amplitude{1};

    RealFFTCalculator fftCalculatorReal(numberOfSamples);
    ComplexFFTCalculator fftCalculatorComplex(numberOfSamples);

    const auto inputData = generateTestSignal(signalFreqency, amplitude);

    const auto fftReal = fftCalculatorReal.calculate(inputData);
    const auto fftComplex = fftCalculatorComplex.calculate(inputData);

    valueChecker(fftReal, fftComplex);
}

TEST_P(FFTCalculatorTest, checkingFftWithSignal1kHz)
{
    const uint32_t signalFreqency{1000};
    const float amplitude{1};

    auto fftCalculator = getCalculator(GetParam(), numberOfSamples);

    const auto inputData = generateTestSignal(signalFreqency, amplitude);

    const auto fft = fftCalculator->calculate(inputData);

    const std::map<Position,ExpectedValue> expectedFftValues = {{0,0}, {1,4}, {2,0},{3,0},{4,0},{5,0},{6,0}, {7,4}};
    const std::map<Position,ExpectedValue> expectedPhaseValues = {{1,-90}, {7,90}};
    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,amplitude}, {2,0},{3,0},{4,0},{5,0},{6,0}, {7,amplitude}};

    verifyFftData(fft, expectedFftValues, expectedFftValuesAfterNormalization, expectedPhaseValues);
}

TEST_P(FFTCalculatorTest, checkingFftWithSignal2kHzAndPhaseOffset)
{
    const uint32_t signalFreqency{2000};
    const uint32_t phaseOffset{135};
    const float amplitude{0.5};

    auto fftCalculator = getCalculator(GetParam(), numberOfSamples);

    const auto inputData = generateTestSignal(signalFreqency, amplitude, phaseOffset);
    const auto fft = fftCalculator->calculate(inputData);

    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,0}, {2,amplitude},{3,0},{4,0},{5,0},{6,amplitude}, {7,0}};
    const std::map<Position,ExpectedValue> expectedPhaseValues = {{2,45}, {6,-45}};

    verifyFftData(fft, {}, expectedFftValuesAfterNormalization, expectedPhaseValues);
}

TEST_P(FFTCalculatorTest, checkingTwoSignalsAddedTogether)
{
    const uint32_t firstSignalFreqency{1000};
    const float firstSignalAmplitude{1};

    const uint32_t secondSignalFreqency{2000};
    const float secondSignalAmplitude{0.5};
    const uint32_t secondSignalPhaseOffset{135};

    auto fftCalculator = getCalculator(GetParam(), numberOfSamples);

    const auto inputFirstSignal = generateTestSignal(firstSignalFreqency, firstSignalAmplitude);
    const auto inputSecondSignal = generateTestSignal(secondSignalFreqency, secondSignalAmplitude, secondSignalPhaseOffset);

    const auto inputData = addSignals(inputFirstSignal, inputSecondSignal);

    const auto fft = fftCalculator->calculate(inputData);

    const std::map<Position,ExpectedValue> expectedFftValues = {{0,0}, {1,4}, {2,2},{3,0},{4,0},{5,0},{6,2}, {7,4}};
    const std::map<Position,ExpectedValue> expectedPhaseValues = {{1,-90},{2,45}, {6,-45}, {7,90}};
    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,firstSignalAmplitude}, {2,secondSignalAmplitude},{3,0},{4,0},{5,0},{6,secondSignalAmplitude}, {7,firstSignalAmplitude}};

    verifyFftData(fft, expectedFftValues, expectedFftValuesAfterNormalization, expectedPhaseValues);
}

INSTANTIATE_TEST_SUITE_P(
    FFTCalculatorTest,
    FFTCalculatorTest,
    ::testing::Values(FftType::Real,FftType::Complex));

class WelchCalculatorTest : public FFTCalculatorTestBase
{
public:
    const float marginOfError = exp(-6);
    const uint32_t numberOfSamples{16};
    const uint32_t maxQueueValue{100};
    const float signalAmplitude{1};

    auto generateWindow(uint32_t numberOfSamples)
    {
        return std::vector<float>(numberOfSamples, 1.0);
    }
};

TEST_P(WelchCalculatorTest, checkingIfCalculatorWorksWellWith50PercentOverlapping)
{
    const float initialOverlapping{0.0};
    const float overlapping{0.5};

    std::vector<float> signal = generateSignal(numberOfSamples,numberOfSamples,signalAmplitude);

    DataExchanger<std::unique_ptr<FFTResult>> queue(maxQueueValue);

    WelchCalculator welchCalculator(GetParam(), numberOfSamples, initialOverlapping, generateWindow(numberOfSamples));
    welchCalculator.updateOverlapping(overlapping);
    welchCalculator.updateBuffer(signal);
    welchCalculator.updateBuffer(signal);
    welchCalculator.calculate(queue);

    EXPECT_EQ(3, queue.getSize());

    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,signalAmplitude}, {2,0},{3,0},{4,0},{5,0},{6,0}, {7,0}, {8,0},{9,0}, {10,0}, {11,0},{12,0},{13,0},{14,0}, {15,signalAmplitude}};

    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,-90}, {15,90}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,90}, {15,-90}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,-90}, {15,90}});
}

TEST_P(WelchCalculatorTest, checkingIfCalculatorWorksWellWith75PercentOverlapping)
{
    const float overlapping{0.75};

    std::vector<float> signal = generateSignal(numberOfSamples,numberOfSamples,signalAmplitude);

    DataExchanger<std::unique_ptr<FFTResult>> queue(maxQueueValue);

    WelchCalculator welchCalculator(GetParam(), numberOfSamples, overlapping, generateWindow(numberOfSamples));
    welchCalculator.updateBuffer(signal);
    welchCalculator.updateBuffer(signal);
    welchCalculator.calculate(queue);

    EXPECT_EQ(5, queue.getSize());

    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,signalAmplitude}, {2,0},{3,0},{4,0},{5,0},{6,0}, {7,0}, {8,0},{9,0}, {10,0}, {11,0},{12,0},{13,0},{14,0}, {15,signalAmplitude}};

    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,-90}, {15,90}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,0}, {15,0}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,90}, {15,-90}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,180}, {15,-180}});
    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,-90}, {15,90}});
}

TEST_P(WelchCalculatorTest, checkingIfCalculatorWorksWellWithOverlappingBelowProperRange)
{
    const float initialOverlapping{0.0};
    const float overlappingBelowProperRange{-1.0};

    std::vector<float> signal = generateSignal(numberOfSamples,numberOfSamples,signalAmplitude);

    DataExchanger<std::unique_ptr<FFTResult>> queue(maxQueueValue);

    WelchCalculator welchCalculator(GetParam(), numberOfSamples, initialOverlapping, generateWindow(numberOfSamples));
    welchCalculator.updateOverlapping(overlappingBelowProperRange);
    welchCalculator.updateBuffer(signal);
    welchCalculator.updateBuffer(signal);
    welchCalculator.calculate(queue);

    EXPECT_EQ(2, queue.getSize());

    const std::map<Position,ExpectedValue> expectedFftValuesAfterNormalization = {{0,0}, {1,signalAmplitude}, {2,0},{3,0},{4,0},{5,0},{6,0}, {7,0}, {8,0},{9,0}, {10,0}, {11,0},{12,0},{13,0},{14,0}, {15,signalAmplitude}};

    verifyFftData(*queue.get(), {}, expectedFftValuesAfterNormalization, {{1,-90}, {15,90}});
}

TEST_P(WelchCalculatorTest, checkingIfCalculatorWorksWellWithOverlappingAboveProperRange)
{
    const float overlappingAboveProperRange{3.0};

    std::vector<float> signal = generateSignal(numberOfSamples,numberOfSamples,signalAmplitude);

    DataExchanger<std::unique_ptr<FFTResult>> queue(maxQueueValue);

    WelchCalculator welchCalculator(GetParam(), numberOfSamples, overlappingAboveProperRange, generateWindow(numberOfSamples));
    welchCalculator.updateBuffer(signal);
    welchCalculator.updateBuffer(signal);
    welchCalculator.calculate(queue);

    EXPECT_EQ(17, queue.getSize());
}

INSTANTIATE_TEST_SUITE_P(
    WelchCalculatorTest,
    WelchCalculatorTest,
    ::testing::Values(FftType::Real,FftType::Complex));
