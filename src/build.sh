#!/usr/bin/sh

set -xe

g++ -Wall -Wextra -o ../bin/$1 $1.cpp
../bin/$1
        