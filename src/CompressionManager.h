#ifndef COMPRESSIONMANAGER_H 
#define COMPRESSIONMANAGER_H

#include <cstdlib>
#include <stdint.h>
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>

#include "compression/Huffman.h"

class CompressionManager {
    private:
        char *file;
        off64_t fsize;

        int32_t *fds;
        int32_t fdnum;

        Compressor *compressor;

    public:
        CompressionManager(char* filename){ 
            
            Initialize(filename);

        };
        
        uint32_t Compress();
        uint8_t Decompress();

        ~CompressionManager(){
            free(file);
            free(fds);
            delete compressor;
        };

    private:
        uint8_t Initialize(char* filename);
        uint8_t ManageFileStream(int32_t fd);
        
};

#endif