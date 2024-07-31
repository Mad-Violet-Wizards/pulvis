#!/bin/bash


# Run executable
if [[ "$1" == "Debug" ]]
then
    echo "[Run] Running Debug build"
    if [[ "$2" == "Game" ]]
    then
        echo "[Run] Running Game."
        ./../../../build/Debug/Game
    elif [[ "$2" == "Playground" ]]
    then
        echo "[Run] Running Playground."
        ./../../../build/Debug/Playground
    else
        echo "[Run] Please specify executable"
        echo "Game"
        echo "Playground"
        echo "ex: ./run.zsh Debug Game"
    fi
elif [[ "$1" == "Release" ]]
then
    echo "[Run] Running Release build"
else
    echo "[Run] Please specify config build"
    echo "Debug"
    echo "Release"
    echo "ex: ./run.zsh Debug Game"
fi
# cd ../../../build/