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
This project uses PyAudio distributed under the MIT License.

**Description:**

SpectrumAnalyzer is an application which collects samples, calculates FFT and visualizes it by means of OpenGL.
Python is mainly used for collecting samples from hw which gives high flexibility of selection source of data.
The usage is not only limited to audio samples analysis. One can update audioConfig.py in such a way that 
it will be possible to read data from other device (for example: SPI accelerometer to measure the frequencies of vibrations).
The file config.py can be used to update such parameters as:
number of strips, frequencies, colors, speed and so on.
What is important recompilation of app is not needed even after updating config.py/audioConfig.py files. 

![spectrumAnalyzer](https://github.com/user-attachments/assets/7df13dbb-58de-47f7-8432-3631ef989950)

![SpectrumAnalyzer version 1 6](https://github.com/user-attachments/assets/e9e815ad-bce6-4449-9a4b-be1e10cc9462)




**Preparing sw environment:**

In order to use it following steps are needed (tested on Ubuntu 24.04):

sudo apt update && apt install -y g++ cmake python3 python3-pyaudio python3-dev libglfw3-dev pkg-config libgtest-dev libgmock-dev wget git

sudo wget http://www.fftw.org/fftw-3.3.10.tar.gz && tar -xzf fftw-3.3.10.tar.gz && cd fftw-3.3.10 && ./configure && make -j4 && make install

git clone https://github.com/sylwekkominek/SpectrumAnalyzer.git

do not forget to configure your input audio device

**Compilation and running:**

cd SpectrumAnalyzer && mkdir build && cd build && cmake .. && make -j4

./spectrum-analyzer

**Compilation of unit tests and running:**

cd SpectrumAnalyzer && mkdir build && cd build && cmake .. -DENABLE_TESTS=ON && make -j4 && cd tests

./spectrum-analyzer-tests

**Docker - running app**

Depending on your system configuration, you may need to adjust the Docker arguments (especially for GUI and audio support).

cd SpectrumAnalyzer

docker build -t spectrum-analyzer -f docker/spectrum-analyzer/Dockerfile .

xhost +local:docker

docker run  -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix   --device /dev/dri  --device /dev/snd   spectrum-analyzer

**Docker - running tests**

cd SpectrumAnalyzer

docker build -t spectrum-analyzer-tests -f docker/spectrum-analyzer-tests/Dockerfile .

docker run spectrum-analyzer-tests
