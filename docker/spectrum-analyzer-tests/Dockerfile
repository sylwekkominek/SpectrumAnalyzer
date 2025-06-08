FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    python3 \
    python3-dev \
    libglfw3-dev \
    pkg-config \
    libgtest-dev \
    libgmock-dev \
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

RUN mkdir build && cd build && cmake .. -DENABLE_TESTS=ON && make -j4

CMD ["bash", "-c", "cd build/tests && ./spectrum-analyzer-tests"]
