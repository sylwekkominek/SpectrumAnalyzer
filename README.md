
# SpectrumAnalyzer

## Description

**SpectrumAnalyzer** is a real-time frequency spectrum visualization tool that uses Python for data acquisition and C++ for data processing and rendering.

By default, the application captures audio samples from the microphone, computes their frequency spectrum using FFT, and visualizes the results with OpenGL graphics.

The architecture is highly flexible — `audioConfig.py` allows you to redefine the data source, making it easy to connect alternative inputs such as SPI sensors (e.g., accelerometers for vibration analysis). This makes the application suitable not only for audio but also for other signal sources.

Visualization parameters — such as bar count, frequency bands, color schemes, and animation speed — are configurable via config.py, with no recompilation required. Users can also generate and customize themes using ChatGPT.

SpectrumAnalyzer is useful for learning, experimentation, and entertainment — whether you're monitoring audio input or other types of signals.


This project is licensed under the **GNU General Public License v2 (GPLv2)**.

It also uses the following third-party libraries:

- PyAudio – MIT License  
- glText – zlib License  
- FFTW – GNU General Public License v2 (GPLv2)  

---

**Full documentation is available in the Wiki:** 
[https://github.com/sylwekkominek/SpectrumAnalyzer/wiki](https://github.com/sylwekkominek/SpectrumAnalyzer/wiki)

**Video on YouTube:** 
[sylwekkominek/SpectrumAnalyzer – Fully Configurable Open Source Spectrum Analyzer](https://www.youtube.com/watch?v=Sjx05eXpgq4)


<img width="1280" height="512" alt="Drawing1" src="https://github.com/user-attachments/assets/a0a98d55-c578-4493-80b8-421b9f5fcd9f" />
<img width="1280" height="512" alt="Drawing13" src="https://github.com/user-attachments/assets/d0592877-32b5-414a-8963-7fd0da590757" />
<img width="1280" height="512" alt="Drawing14" src="https://github.com/user-attachments/assets/78be2ed1-ef67-4b6d-b01d-6ce9482d43c2" />
<img width="1280" height="512" alt="Drawing15" src="https://github.com/user-attachments/assets/3d1422b1-ce85-4334-ba17-655c9b6e149e" />
<img width="1280" height="512" alt="Drawing16" src="https://github.com/user-attachments/assets/3032b880-b481-41ac-8692-ac4809d5c088" />
<img width="1280" height="512" alt="Drawing17" src="https://github.com/user-attachments/assets/d4b41be7-b6b1-4d63-aa22-050c72a04e58" />

## Preparing the software environment

### For typical users:

In order to use it following steps are needed (tested on Ubuntu 24.04). It was also tested on Windows 11 using WSL2, although full performance may not be achievable in that environment:
```bash
sudo apt update && sudo apt install -y g++ cmake python3 python3-pyaudio python3-dev libglfw3-dev libfftw3-dev pkg-config git

git clone https://github.com/sylwekkominek/SpectrumAnalyzer.git
```

**Compilation and running:**
```bash
cd SpectrumAnalyzer && mkdir build && cd build && cmake .. && make -j4

(This step is only required for Raspberry Pi users: export MESA_GL_VERSION_OVERRIDE=4.5)

./spectrum-analyzer
```
do not forget to configure your input audio device

---
### For advanced users:

**Compilation of unit tests and running:**
```bash
sudo apt update && sudo apt install -y libgtest-dev libgmock-dev
cd SpectrumAnalyzer && mkdir build && cd build && cmake .. -DENABLE_TESTS=ON && make -j4 && cd tests
./spectrum-analyzer-tests
```
**Docker - running app**

Depending on your system configuration, you may need to adjust the Docker arguments (especially for GUI and audio support).
```bash
cd SpectrumAnalyzer

docker build -t spectrum-analyzer -f docker/spectrum-analyzer/Dockerfile .

xhost +local:docker

docker run  -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix   --device /dev/dri  --device /dev/snd   spectrum-analyzer
```
**Docker - running tests**
```bash
cd SpectrumAnalyzer

docker build -t spectrum-analyzer-tests -f docker/spectrum-analyzer-tests/Dockerfile .

docker run spectrum-analyzer-tests
```
