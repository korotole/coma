#ifndef COMPRESSIONMANAGER_H 
#define COMPRESSIONMANAGER_H

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>
#include <assert.h>

#include "FileStream.h"
#include "../conf/conf.h"
#include "action/RLE.h"

class CompressionManager {

    private:
        FileStream *input = nullptr;
        FileStream *output = nullptr;
        struct Options {
            char infile[128]    = FILE_PREFIX;
            char outfile[128]   = FILE_PREFIX;
            uint8_t mode        = 0;
            ssize_t (*action)(FileStream*, FileStream*);
        } opts;

    public:
        CompressionManager(int argc, char** argv){ 
            if(!ParseOptions(argc, argv) || Initialize() > 0)
            {
                exit(EXIT_FAILURE);
            }
        };

        ~CompressionManager(){ 
            if(input != nullptr) delete input;
            if(output != nullptr) delete output;
        };

        ssize_t DoWork();

    private:

        uint8_t Initialize();

        void PrintHelpMessage(char* msg=NULL);
        bool ParseOptions(int argc, char **argv);
        
};

#endif
