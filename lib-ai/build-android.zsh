#!/bin/zsh

if [[ "$1" == "--clean" ]]; then
    rm -rf ./build/*
    cmake -B build -DCMAKE_TOOLCHAIN_FILE=/Users/fred/Library/Android/sdk/ndk/28.0.13004108/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-21 -DCMAKE_BUILD_TYPE=Release
fi

cd build
make -j$(sysctl -n hw.ncpu)
cp libquickmaths.so ../../android/app/src/main/jniLibs/arm64-v8a/libquickmaths.so
