#!/bin/bash

if [[ "$1" == "Debug" ]]
then
    echo "[Run] Running Debug build"
    ./../../../build/Debug/Game
elif [[ "$1" == "Release" ]]
then
    echo "[Run] Running Release build"
else
    echo "[Run] Please specify config build"
    echo "Debug"
    echo "Release"
    echo "ex: ./run.zsh Debug"
fi
# cd ../../../build/