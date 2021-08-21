#!/bin/bash

cmake -S . -B ./out/build/
CMAKE=$?

if [[ ${CMAKE} != 0 ]]; then
    printf "\n"
    echo "ERROR:  cmake"
    exit 1
fi

make -C ./out/build/
MAKE=$?
if [[ ${MAKE} != 0 ]]; then 
    printf "\n"
    echo "ERROR:  make"
    exit 2
fi

mv ./out/build/coma ./out/

printf "\n"
echo "SUCCESS"