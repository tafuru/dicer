FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

RUN DEBIAN_FRONTEND=noninteractive apt-get install -U -y --no-install-recommends \
    python3 \
    python3-pip \
    ninja-build \
    ccache \
    nodejs \
    npm \
  && rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-cache-dir --break-system-packages \
    cmake_format \
    pyyaml

RUN npm install -g \
    cspell@latest

WORKDIR /workspaces

RUN git clone --depth 1 -b release https://github.com/Kitware/CMake.git \
  && cd CMake \
  && ./bootstrap --prefix=/usr \
  && make -j$(nproc) install \
  && cd .. \
  && rm -rf CMake

RUN git clone --depth 1 -b p2996 https://github.com/bloomberg/clang-p2996.git llvm-project \
  && cd llvm-project \
  && cmake -G Ninja -S llvm -B build \
    -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
    -DLLVM_BUILD_TOOLS="OFF" \
    -DLLVM_BUILD_LLVM_DYLIB="YES" \
    -DCMAKE_BUILD_TYPE="MinSizeRel" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
  && cmake --build build --target install \
  && cmake -G Ninja -S runtimes -B build-runtimes \
    -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" \
    -DCMAKE_BUILD_TYPE="MinSizeRel" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
  && cmake --build build-runtimes --target install-cxx install-cxxabi install-unwind \
  && cd .. \
  && rm -rf llvm-project

RUN git clone --depth 1 -b stable https://github.com/rui314/mold.git \
  && cd mold \
  && cmake -G Ninja -S . -B build \
    -DCMAKE_BUILD_TYPE="MinSizeRel" \
    -DCMAKE_INSTALL_PREFIX="/usr" \
  && cmake --build build --target install \
  && cd .. \
  && rm -rf mold
