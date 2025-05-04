**Copyright (C) 2024-2025, Sylwester Kominek**

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see the LICENSE file for details on copying and usage.


**Description:**

SpectrumAnalyzer is an application which collects samples, calculates FFT and visualizes it by means of OpenGL.
Python is mainly used for collecting samples from hw which gives high flexibility of selection source of data.
The usage is not only limited to audio samples analysis. One can update audioConfig.py in such a way that 
it will be possible to read data from other device (for example: SPI accelerometer to measure the frequencies of vibrations).
The file config.py can be used to update such parameters as:
number of strips, frequencies, colors, speed and so on.
What is important recompilation of app is not needed even after updating config.py/audioConfig.py files. 


**Preparing sw environment:**

In order to use it following steps are needed (tested on Ubuntu 24.04):

sudo apt update

sudo apt install g++

sudo apt install cmake

sudo apt install python3

sudo apt install python3-pyaudio

sudo apt install python3-dev

sudo apt install libglfw3-dev

sudo apt install pkg-config

sudo apt install libgtest-dev

sudo apt install libgmock-dev

download fftw from https://www.fftw.org/download.html extract and run commands:

./configure

make 

sudo make install

do not forget to configure your input audio device

**Compilation and running:**

download SpectrumAnalyzer

mkdir build

cd build

cmake ..

make -j4

./spectrum-analyzer

**Compilation of unit tests and running:**

mkdir build

cd build

cmake .. -DENABLE_TESTS=ON

make -j4

cd tests

./spectrum-analyzer-tests


