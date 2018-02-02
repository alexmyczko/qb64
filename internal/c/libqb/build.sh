#!/bin/bash
cd libqb

for file in $(find src -name '*.cpp')
    do g++ "$@" -c -s -w -Wall -I .. -I include -I include/$(dirname $file) $file -o os/lnx/$(basename ${file%.cpp}.o)
done

ar rcs os/lnx/libqb.a $(ls os/lnx/*.o)

