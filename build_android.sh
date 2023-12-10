#/bin/bash
# replace to your ndk path
export ANDROID_NDK=/home/code01/桌面/project/env/android-ndk-r26b 
rm -rf build/*
cd build
cmake -DTARGET=android -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_NDK=$ANDROID_NDK \
    -DANDROID_PLATFORM=android-28 \
    ..
make
