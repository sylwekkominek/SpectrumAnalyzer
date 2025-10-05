# Copyright (C) 2024-2025, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

import math

#To enable this script, you need to edit the getPythonDataSourceEnabled function in the config.py file to return True.

#The variable numberOfSamples should be set to dataLength. However, to keep the script simple for generating a sine wave, this value is being overwritten with a fixed number (4096).
def initialize(dataLength, samplingRate):
    global numberOfSamples
    global fs

    numberOfSamples = 4096
    fs = samplingRate
    return True

#The frequency 1001.29 Hz is chosen because the sampling rate (44100 Hz) divided by the data length (4096) is approximately 10.76 Hz. The value 1001.29 Hz is a multiple of this 10.76 Hz to minimize spectral leakage to other frequencies. This ensures that the amplitude aligns correctly with the scale in dBFS
def getData():
    signalFrequency = 1001.29 #Hz
    samplingPeriod = 1.0 / fs
    fullScaleAmplitude = 32767

    result = tuple()

    for n in range(numberOfSamples):
        y = fullScaleAmplitude * math.sin(2 * math.pi * n * signalFrequency * samplingPeriod)
        result += (y,)

    return result
