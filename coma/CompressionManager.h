#include <cstdlib>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../datatypes.h"

class CompressionManager {
    public:
        CompressionManager(){ 

        }
        uint32_t zip(std::string filename);
        uint8_t unzip(std::string filename);

        ~CompressionManager(){

        };

    private:
        void openFile(std::string filename);
        

};