#include <cstdio>
#include <iostream>
#include "CompressionManager.h"

int main(int argc, char** argv) {

    CompressionManager coma = CompressionManager(argc, argv);
    coma.DoWork();

    return 0;
}
