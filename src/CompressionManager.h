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
#include "Common.h"
#include "action/RLE.h"

class CompressionManager {

    private:
        FileStream input;
        FileStream output;
        struct Options {
            char infile[128]    = "\0";
            char outfile[128]   = "\0";
            uint8_t mode        = 0;
            uint64_t (*action)(FileStream&, FileStream&);
        } opts;

    public:
        CompressionManager(int argc, char** argv){ 
            if(!ParseOptions(argc, argv))
            {
                exit(EXIT_FAILURE);
            }

            Initialize();
        };

        ~CompressionManager() { };

        size_t DoWork();

    private:

        uint8_t Initialize();

        void PrintHelpMessage(char* msg=NULL);
        bool ParseOptions(int argc, char **argv);
        
};

#endif