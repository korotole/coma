#!/bin/bash

# visual effects
BOLD=`tput bold`
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

rm -f ./out/valgrind-out.txt

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=./out/valgrind-out.txt \
         ./out/coma -i ./out/test -o ./out/outfile -m c > ./out/out.txt      #   <--

BOLD=`tput bold`

ERRORS=$(grep -Po -m 1 "Error summary: \d+" ./out/valgrind-out.txt)
SUCCESS=$(grep -Po -m 1 "\d+" <<< "${ERRORS}")

if [[ ${SUCCESS} != 0 ]]; then
    echo "${RED}${BOLD}${ERRORS}${NC}"
fi