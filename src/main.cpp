#include <cstdio>
#include <iostream>
#include "CompressionManager.h"

int main(int argc, char** argv) {

    CompressionManager coma = CompressionManager(argc, argv);
    printf("result: %zu\n", coma.DoWork());

    return 0;
}
