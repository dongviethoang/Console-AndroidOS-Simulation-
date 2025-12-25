#!/bin/bash

echo "Welcome to Console AndroidOS Terminal"
echo "Type help for help."

read -p "$ ~ >> " input

if [[ "$input" == "pacman -S conandpkg.zip" ]]; then
    wget "https://github.com/beta7developer/src-conand/archive/refs/heads/bin.zip"
    wget "https://github.com/beta7developer/src-conand/archive/refs/heads/retro.zip"
    wget "https://github.com/beta7developer/src-conand/archive/refs/heads/scrapped.zip"
    wget "https://github.com/beta7developer/src-conand/archive/refs/heads/src.zip"
else
    echo "bash: invalid command"
fi