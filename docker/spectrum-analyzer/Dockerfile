FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-dev \
    python3-pyaudio \
    libglfw3-dev \
    libglfw3 \
    libgl1 \
    libgl1-mesa-dri \
    libglu1-mesa \
    libpulse-dev \
    alsa-utils \
    pkg-config \
    wget

WORKDIR /tmp
RUN wget http://www.fftw.org/fftw-3.3.10.tar.gz && \
    tar -xzf fftw-3.3.10.tar.gz && \
    cd fftw-3.3.10 && \
    ./configure && \
    make -j4 && \
    make install

WORKDIR /app
COPY . /app

RUN mkdir build && cd build && cmake .. && make -j4

CMD ["bash", "-c", "./build/spectrum-analyzer"]

