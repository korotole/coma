#include "archman.h"

uint32_t ArchMan::zip(std::string filename){
    Node a;
    return filename.length();
}

void ArchMan::openFile(std::string filename){

    struct stat sb;
    if (stat(filename.c_str(), &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    void* fileContents = calloc(1, sb.st_size);

    int32_t fd = open64(filename.c_str(), O_RDONLY);
}