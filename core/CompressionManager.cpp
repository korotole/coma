#include "CompressionManager.h"

uint32_t CompressionManager::zip(std::string filename){
    std::cout << "Hello from a compresor!" << std::endl;
    return filename.length();
}

void CompressionManager::openFile(std::string filename){

    struct stat sb;
    if (stat(filename.c_str(), &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    void* fileContents = calloc(1, sb.st_size);


    int32_t fd = open64(filename.c_str(), O_RDONLY);
}
