# Copyright (C) 2024-2025, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

# Full spec: https://github.com/sylwekkominek/SpectrumAnalyzer/wiki

import struct
import pyaudio

p = pyaudio.PyAudio()


#Description: This function prepares the hardware or data source to capture chunks of data of size dataLength at the specified samplingRate. It opens an input stream using PyAudio configured for 16-bit stereo input and sets up the system to collect data in real-time. Although designed for audio input, this function is flexible — users can replace or extend it to work with other data sources, such as reading from a WAV file, accelerometer data, or any other stream.
#Default value: True on success

def initialize(dataLength, samplingRate):
    global chunk
    global inputStream
    chunk = dataLength
    inputStream = p.open(format = pyaudio.paInt16, channels=2,rate=samplingRate, input=True, frames_per_buffer=chunk)
    return True


#Description: This function reads a block of data from the input device. The data contains samples for both left and right audio channels (stereo). It converts the raw bytes into a list of 16-bit signed numbers, alternating between left and right channel samples. This way, you get the latest stereo audio data ready to use.
#Default value: (Gets the latest data from hardware for left and right channels.)

def getData():
    data = inputStream.read(chunk, exception_on_overflow=False)
    return (struct.unpack('{n}h'.format(n= len(data)//2 ), data))
