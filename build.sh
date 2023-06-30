#!/bin/sh

mkdir build
gcc main.c -o build/main -I ./include -L ./lib -lSDL2
./build/main