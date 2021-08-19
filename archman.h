#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "datatypes.h"

class ArchMan {
    public:
        ArchMan(){ 

        }
        uint32_t zip(std::string filename);
        uint8_t unzip(std::string filename);

        ~ArchMan(){

        };

    private:
        void openFile(std::string filename);
        

};