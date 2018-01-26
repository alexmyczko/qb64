#!/bin/bash
cd libqb
for file in $(ls src)
    do g++ "$@" -c -s -w -Wall -I .. -I include src/$file -o os/lnx/${file%.cpp}.o
done

ar rcs os/lnx/libqb.a $(ls os/lnx/*.o)

