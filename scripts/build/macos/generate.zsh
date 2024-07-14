#!/bin/bash

echo "[Generate] Starting to generate project files."

case "$(uname -s)" in
    "Darwin")
        echo "[Generate] Setting environment for MacOS"
        if [[ "$(uname -m)" == "arm64" ]]
        then 
            echo "[Generate] Running on arm64 chip"
            ./premake5 gmake2 --file=../../../premake5.lua
        else
            echo "[Generate] running on Intel chip"
            echo "[Generate] Not implemented yet."
        fi
;;
    "Linux")
        echo "[Generate] Setting environment for Linux"
        echo "[Generate] Script run from MacOS directory. Are you sure what you are doing? :D"
;;
esac
