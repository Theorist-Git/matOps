#!/usr/bin/env bash

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++11 -Wall -Wextra -Werror -pedantic -O2"

# Output executable
OUT="bin/testMatrix"

# Create bin directory if it doesn't exist
mkdir -p bin

echo "Compiling testMatrix.cpp -> $OUT"
# Compile testMatrix.cpp to an object file
$CXX $CXXFLAGS -c src/testMatrix.cpp -o bin/testMatrix.o

# Link the object file(s) into an executable
$CXX bin/testMatrix.o -o $OUT

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful. Running tests..."
    ./$OUT
else
    echo "Build failed."
    exit 1
fi