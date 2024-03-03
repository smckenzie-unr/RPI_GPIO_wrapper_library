#!/bin/sh
DIRECTORY=build
if [ -d "$DIRECTORY" ]; then
    if [ "$(ls $DIRECTORY)" ]; then  
        echo "[\33[1;32mSuccess\33[0m]       Removed build directory."
        rm -r "$DIRECTORY"/*
    fi
fi
make