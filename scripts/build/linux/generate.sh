#!/bin/bash

echo "[Generate] Starting to generate project files."

macos_arm_path="/macos/arm"

case "$(uname -s)" in
    "Darwin")
        echo "[Generate] Setting environment for MacOS"
        if [[ "$(uname -m)" == "arm64" ]]
        then 
            echo "[Generate] Running on arm64 chip"
            ./macos/premake5 make --file=../../premake5.lua
        else
            echo "[Generate] running on Intel chip"
            echo "[Generate] No script specified"
        fi
;;
    "Linux")
        echo "[Generate] Setting environment for Linux"
        echo "[Generate] No script specified"
;;
esac
