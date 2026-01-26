
# SpectrumAnalyzer

## Description

**SpectrumAnalyzer** is a real-time frequency spectrum visualization tool written primarily in C++ for data processing and rendering, with optional Python-based audio acquisition.

By default, the application captures audio from the microphone via PortAudio, computes its frequency spectrum using FFT, and visualizes the results with OpenGL graphics. When built with the -DENABLE_LOOPBACK=on flag, it can capture system playback audio (Spotify, YouTube, and other applications), allowing analysis of sounds being played on your system.

The architecture is flexible — although processing and visualization are done in C++, audioConfig.py lets users replace the default input with any Python-defined source, such as playback streams, virtual devices, or custom sensors.

The app allows you to customize parameters — such as bar count, frequency ranges, and colors — using simple text configuration files stored in the config folder.

If something goes wrong, you can delete a file or the entire folder, and the app will recreate default settings. Each configuration file includes a description of its purpose. You can also create your own custom color themes if the default ones are not enough. For additional help, you can copy the file contents into ChatGPT for guidance.

SpectrumAnalyzer is useful for learning, signal analysis, experimentation, and entertainment — whether analyzing live microphone input, playback audio, or other types of signals.
This project is licensed under the **GNU General Public License v2 (GPLv2)**.

It also uses the following third-party libraries:

- PortAudio – MIT License  
- glText – zlib License  
- FFTW – GNU General Public License v2 (GPLv2)  

---

## A ready-to-download version for Windows is available here:

- **Microphone input analysis**  
  [Microsoft Store – Not Yet Another Spectrum Analyzer](https://apps.microsoft.com/detail/9ngnl3tjj7bg?hl=en-US&gl=US)

- **Analysis of what is currently being played** (system audio such as Spotify, YouTube, etc.)  
  [Microsoft Store – Not Yet Another Music Visualizer](https://apps.microsoft.com/detail/9n6f33xks0k9?hl=en-US&gl=US)


---

**Full documentation is available in the Wiki:** 
[https://github.com/sylwekkominek/SpectrumAnalyzer/wiki](https://github.com/sylwekkominek/SpectrumAnalyzer/wiki)

**Video on YouTube:** 
[sylwekkominek/SpectrumAnalyzer – Fully Configurable Open Source Spectrum Analyzer](https://www.youtube.com/watch?v=Sjx05eXpgq4)

![output](https://github.com/user-attachments/assets/0b94cb50-937a-4331-a8e6-710d2abcc987)

<img width="1280" height="512" alt="Drawing1" src="https://github.com/user-attachments/assets/a0a98d55-c578-4493-80b8-421b9f5fcd9f" />
<img width="1280" height="512" alt="Drawing13" src="https://github.com/user-attachments/assets/d0592877-32b5-414a-8963-7fd0da590757" />
<img width="1280" height="512" alt="Drawing14" src="https://github.com/user-attachments/assets/78be2ed1-ef67-4b6d-b01d-6ce9482d43c2" />
<img width="1280" height="512" alt="Drawing15" src="https://github.com/user-attachments/assets/3d1422b1-ce85-4334-ba17-655c9b6e149e" />
<img width="1280" height="512" alt="Drawing16" src="https://github.com/user-attachments/assets/3032b880-b481-41ac-8692-ac4809d5c088" />
<img width="1280" height="512" alt="Drawing17" src="https://github.com/user-attachments/assets/d4b41be7-b6b1-4d63-aa22-050c72a04e58" />

---

## For programmers:


### For typical users: Ubuntu 24.04 or Windows 11 with WSL2.

```bash
sudo apt update && sudo apt install -y g++ cmake python3 python3-dev libglfw3-dev portaudio19-dev libfftw3-dev pkg-config git

git clone https://github.com/sylwekkominek/SpectrumAnalyzer.git
```

**Compilation and running:**
```bash
cd SpectrumAnalyzer && mkdir build && cd build && cmake .. && cmake --build . -j 4

(This step is only required for Raspberry Pi users: export MESA_GL_VERSION_OVERRIDE=4.5)

./spectrum-analyzer
```
do not forget to configure your input audio device
By default, the application runs in fullscreen mode with a resolution of 1920x1080 (Full HD). You can change this resolution by editing the config.py file.

---

### For typical users: Windows

Download and install msys2 from https://www.msys2.org/#installation

Open msys2 terminal and run:

```bash
 pacman -Syu
```

Close msys2 terminal and reopen it

```bash
pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-python mingw-w64-x86_64-fftw mingw-w64-x86_64-glfw mingw-w64-x86_64-portaudio git

```
**Compilation and running:**

If you are using a newer Python version, you need to specify a different folder name.

```bash
export PATH=/C/msys64/mingw64/bin:$PATH
export PYTHONHOME=/C/msys64/mingw64
export PYTHONPATH=/C/msys64/mingw64/lib/python3.12:/C/msys64/mingw64/lib/python3.12/lib-dynload

git clone https://github.com/sylwekkominek/SpectrumAnalyzer.git
cd SpectrumAnalyzer && mkdir build && cd build && cmake .. && cmake --build . -j 4
./spectrum-analyzer
```

---
### For advanced users:

**Compilation of unit tests and running:**
```bash
sudo apt update && sudo apt install -y libgtest-dev libgmock-dev
cd SpectrumAnalyzer && mkdir build && cd build && cmake .. -DENABLE_TESTS=ON && make -j4 && cd tests
./spectrum-analyzer-tests
```
**Docker - Running an App with Microphone**

Depending on your system configuration, you may need to adjust the Docker arguments (especially for GUI and audio support).
```bash
cd SpectrumAnalyzer

docker build -t spectrum-analyzer -f docker/spectrum-analyzer/Dockerfile .

xhost +local:docker

docker run  -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix   --device /dev/dri  --device /dev/snd   spectrum-analyzer
```
**Docker - Running an App with Audio Loopback**

Depending on your system configuration, you may need to adjust the Docker arguments (especially for GUI and audio support).
```bash
cd SpectrumAnalyzer

docker build -t spectrum-analyzer-loopback -f docker/spectrum-analyzer-loopback/Dockerfile .

xhost +local:docker

docker run -it --device /dev/dri --device /dev/snd -v /tmp/.X11-unix:/tmp/.X11-unix -v $XDG_RUNTIME_DIR/pulse:/run/user/1000/pulse -e DISPLAY=$DISPLAY -e PULSE_SERVER=unix:/run/user/1000/pulse/native spectrum-analyzer-loopback
```

**Docker - running tests**
```bash
cd SpectrumAnalyzer

docker build -t spectrum-analyzer-tests -f docker/spectrum-analyzer-tests/Dockerfile .

docker run spectrum-analyzer-tests
```
