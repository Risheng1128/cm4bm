#!/usr/bin/env bash

# GNU toolchain for ARM 32 bit processor
GCC_VER=12.2.rel1
TOOLCHAIN=arm-none-eabi
TOOLCHAIN_URL=https://developer.arm.com/-/media/Files/downloads/gnu

# print commands and arguments as they are executed.
set -x

# download ARM toolchain
wget -q \
    ${TOOLCHAIN_URL}/${GCC_VER}/binrel/arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}.tar.xz

tar Jxvf arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}.tar.xz

export PATH=`pwd`/arm-gnu-toolchain-${GCC_VER}-x86_64-${TOOLCHAIN}/bin:$PATH

# build
make