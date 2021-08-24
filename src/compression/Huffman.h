#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <stdint.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../../conf/conf.h"
#include "../datatypes.h"
#include "../../conf/conf.h"
#include "Compressor.h"

namespace huffman {

    typedef struct HuffmanNode {
        uint8_t a;
        void* left;
        void* right;
    } HuffmanNode;
 
    class Huffman : public Compressor {
        private:
            std::map<uint8_t, uint32_t> distribution = {};
            int32_t fd;
        
        
        public:

            Huffman(int32_t fd) {
                this->fd = fd;
            };

            void Compress(void* stream);
            void Decompress(void* stream);

            ~Huffman() {
                
            }
        
        private:
            int32_t ComputeEntryDistribution(int32_t fd);
            bool CheckMap(std::map<uint8_t, uint32_t> m, uint8_t key);
        
    };

};

#endif