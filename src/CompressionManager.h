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
#include "datatypes.h"

class CompressionManager {
    public:
        char* file;
        void* contents;

        CompressionManager(char* filename){ 
            file = (char *) calloc(1, strlen(filename)+1);
            strcpy(file, filename);

            contents = NULL;
        }
        
        uint32_t compress();
        uint8_t decompress();

        ~CompressionManager(){
            free(file);
            if( contents ) free(contents);
        };

    private:
        void openFile();
        

};