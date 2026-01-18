FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    git \
    python3 \
    python3-dev \
    python3-pyaudio \
    libglfw3-dev \
    libfftw3-dev \
    libgl1 \
    libgl1-mesa-dri \
    libglu1-mesa \
    libpulse-dev \
    libasound2-dev \
    libjack-jackd2-dev \
    alsa-utils \
    pkg-config

WORKDIR /tmp
RUN git clone https://github.com/PortAudio/portaudio.git && \
    cd portaudio && \
    ./configure --with-pulseaudio && \
    make -j4 && \
    make install && \
    ldconfig

WORKDIR /app
COPY . /app

RUN mkdir build && cd build && cmake .. -DENABLE_LOOPBACK=on && make -j4

CMD ["bash", "-c", "./build/spectrum-analyzer"]

