FROM ubuntu:impish

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive && \
  apt-get install -qy \
  build-essential \
  cmake \
  gcc-arm-none-eabi \
  git \
  libnewlib-arm-none-eabi \
  libstdc++-arm-none-eabi-newlib \
  ninja-build \
  python3 \
  && apt-get autoremove -y \
  && apt-get clean -y \
  && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/raspberrypi/pico-sdk.git /pico-sdk \
  && cd /pico-sdk \
  && git switch -c raphaelmeyer/1.3.0 1.3.0 \
  && git submodule update --init

ENV PICO_SDK_PATH /pico-sdk

RUN useradd -ms /bin/bash vscode
RUN mkdir /build && chown -R vscode:vscode /build
USER vscode

VOLUME [ "/build" ]
