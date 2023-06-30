#!/bin/sh

if [ ! -d "./build" ]
then
    mkdir build
else
    rm -rf ./build
    mkdir build
fi
gcc main.c -o build/AsteroidZ -I ./include -L ./lib -lSDL2 -v
./build/AsteroidZ