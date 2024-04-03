#!/bin/bash
if [ ! -d "build" ]; then
    mkdir build
fi
./build_linux.sh
./build_android.sh
