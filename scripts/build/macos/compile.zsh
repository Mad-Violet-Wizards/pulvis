#!/bin/bash

echo "[Compile] Starting to generate project files."

case "$(uname -s)" in
    "Darwin")
        echo "[Compile] Compiling for MacOS."
        if [[ "$(uname -m)" == "arm64" ]]
        then
            cd ../../../
            make all
            cd scripts/build/macos
            echo "[Compile] Done."
        else
            echo "[Compile] Version for Intel Mac not prepared yet."
        fi
;;
esac
