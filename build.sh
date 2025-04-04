#!/bin/sh
rm -rf build/
mkdir build && cd build
cmake -DPICOTOOL_FORCE_FETCH_FROM_GIT=ON ..
make -j4
