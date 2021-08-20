#include <cstdio>
#include <iostream>
#include "CompressionManager.h"

int main(int argc, char** argv) {

    std::cout << "Hello World!" << std::endl;
    
    CompressionManager coma = CompressionManager();
    coma.zip("koko");

    return 0;
}
