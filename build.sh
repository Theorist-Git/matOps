#!/usr/bin/env bash

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -O2"

# Output executable
OUT="bin/testMatrix"

# Create bin directory if it doesn't exist
mkdir -p bin

echo "Compiling testMatrix.cpp (which includes linAlg.cpp) -> $OUT"
$CXX $CXXFLAGS src/testMatrix.cpp -o $OUT

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful. Running tests..."
    ./$OUT
else
    echo "Build failed."
    exit 1
fi