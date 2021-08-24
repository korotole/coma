rm -f ./out/valgrind-out.txt

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=./out/valgrind-out.txt \
         ./out/coma ./out/t > out1.txt

BOLD=`tput bold`

ERRORS=$(grep -Po -m 1 "ERROR SUMMARY: \d+" ./out/valgrind-out.txt)
SUCCESS=$(grep -Po -m 1 "\d+" <<< "${ERRORS}")

if [[ ${SUCCESS} != 0 ]]; then
    echo ${BOLD}${ERRORS}
fi