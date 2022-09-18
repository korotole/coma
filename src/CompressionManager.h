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

typedef struct {
    int32_t fd1;
    int32_t fd2;
    off64_t fileOffset1;
    off64_t fileOffset2;
    off64_t amount;
} params_t;

class CompressionManager {
    private:
        struct Options {
            char infile[128]    = "\0";
            char outfile[128]   = "\0";
            uint8_t mode        = 0;
        } opts;

        params_t params = { 0 };

    public:
        CompressionManager(int argc, char** argv){ 
            if(!ParseOptions(argc, argv))
            {
                exit(EXIT_FAILURE);
            }

            Initialize();
        };

        ~CompressionManager(){ };

        off64_t DoWork();

    private:
        int32_t Compress();
        int32_t Decompress();

        uint8_t Initialize();

        void PrintHelpMessage(char* msg=NULL);
        bool ParseOptions(int argc, char **argv);
        
};

#endif