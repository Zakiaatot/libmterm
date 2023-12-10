#/bin/bash
rm -rf build/*
cd build
cmake -DTARGET=linux ..
make
