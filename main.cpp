#include <cstdio>
#include <iostream>
#include "archman.h"

int main(int argc, char** argv) {
    ArchMan archman = ArchMan();
    std::cout << archman.zip(argv[1]) << std::endl;
    return 0;
}