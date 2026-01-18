FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-dev \
    libglfw3-dev \
    portaudio19-dev \
    libfftw3-dev \
    pkg-config \
    libgtest-dev \
    libgmock-dev
WORKDIR /app
COPY . /app

RUN mkdir build && cd build && cmake .. -DENABLE_TESTS=ON && make -j4

CMD ["bash", "-c", "cd build/tests && ./spectrum-analyzer-tests"]
