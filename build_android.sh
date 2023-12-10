#/bin/bash
# replace to your ndk path
export ANDROID_NDK=/home/code01/桌面/project/env/android-ndk-r26b

TARGET_ABI=("arm64-v8a" "armeabi-v7a" "x86_64" "x86")

for ABI in "${TARGET_ABI[@]}"; do
    rm -rf build/*
    cd build
    cmake -DTARGET=android -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=$ABI \
        -DANDROID_NDK=$ANDROID_NDK \
        -DANDROID_PLATFORM=android-28 \
        ..
    make -j$(nproc)
    cd ..
done
