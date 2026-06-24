#!/bin/bash

set -e

cmake -S . -B build
cmake --build build -j$(sysctl -n hw.ncpu)
ctest --test-dir build --output-on-failure