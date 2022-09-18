#!/bin/bash

# visual effects
BOLD=`tput bold`
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

rm -rf ./out/build/
mkdir --mode=0777 ./out/build/

if [[ ${1} == "-r" ]]; then
    echo -e "Removing old files..."
    rm -rf ./out/
fi

cmake -S . -B ./out/build/
CMAKE=$?

if [[ ${CMAKE} != 0 ]]; then
    echo -e "${RED}${BOLD}Error:  cmake (${CMAKE})${NC}"
    exit 1
fi

make -C ./out/build/
MAKE=$?

if [[ ${MAKE} != 0 ]]; then 
    echo -e "${RED}${BOLD}Error:  make (${MAKE})${NC}"
    exit 2
fi

mv ./out/build/coma ./out/

echo -e "${GREEN}${BOLD}Success${NC}"