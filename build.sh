#!/usr/bin/env bash

set -xe

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++11 -fopenmp -Wall -Wextra -Werror -pedantic -O3"

# Output executable
OUT="bin/testMatrix"

# Create bin directory if it doesn't exist
mkdir -p bin

echo "Compiling testMatrix.cpp -> $OUT"
# Compile testMatrix.cpp to an object file
$CXX $CXXFLAGS -c test/testMatrix.cpp -o bin/testMatrix.o

# Link the object file(s) into an executable
$CXX $CXXFLAGS bin/testMatrix.o -o $OUT

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful. Running tests..."
    ./$OUT
else
    echo "Build failed."
    exit 1
fi