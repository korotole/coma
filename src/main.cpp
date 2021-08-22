#include <cstdio>
#include <iostream>
#include "CompressionManager.h"

int main(int argc, char** argv) {

    std::cout << "Hello World!" << std::endl;
    
    CompressionManager coma = CompressionManager(argv[1]);
    coma.Compress();

    return 0;
}
