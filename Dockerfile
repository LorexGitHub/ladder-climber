FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libvorbis-dev \
    libogg-dev \
    libflac-dev \
    fonts-dejavu-core \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /project

COPY . .

WORKDIR /project/app/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make -j$(nproc) DonkeyKong_test DonkeyKong

WORKDIR /project/app
CMD ["./build/bin/DonkeyKong"]
