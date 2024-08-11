#!/bin/bash

file_extensions=("*.make" "Makefile", "*.pcm")

echo "[Clean] Running clean script."

for file_extension_to_rm in ${file_extensions[@]}; do
    find ../../.. -name ${file_extension_to_rm} -prune -exec zsh -c 'echo "Deleted file: {}" && rm -rf {}' \;
done

rm -rf ../../../build/*
echo "[Clean] Build directory cleaned!"
rm -rf ../../../obj
echo "[Clean] Obj directory removed.";
