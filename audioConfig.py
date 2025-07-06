# Copyright (C) 2024, Sylwester Kominek
# This file is part of SpectrumAnalyzer program licensed under GPLv2 or later,
# see file LICENSE in this source tree.

import struct
import pyaudio

p = pyaudio.PyAudio()

def initialize(dataLength, samplingRate):
    global chunk
    global inputStream
    chunk = dataLength
    inputStream = p.open(format = pyaudio.paInt16, channels=2,rate=samplingRate, input=True, frames_per_buffer=chunk)
    return True

def getData():
    data = inputStream.read(chunk, exception_on_overflow=False)
    return (struct.unpack('{n}h'.format(n= len(data)//2 ), data))
