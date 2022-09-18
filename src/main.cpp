#include <cstdio>
#include <iostream>
#include "CompressionManager.h"

int main(int argc, char** argv) {

    std::cout << "Hello World!" << std::endl;
    
    CompressionManager coma = CompressionManager(argc, argv);
    coma.DoWork();

    return 0;
}
