#include <map>
#include <stdint.h>
#include <iostream>

#include "Compressor.h"

namespace huffman {

    typedef struct HuffmanNode {
        uint8_t a;
        void* left;
        void* right;
    } HuffmanNode;
 
    class Huffman : public Compressor {
        private:
            std::map<uint8_t, uint32_t> distribution;
        
        
        public:

            Huffman() {

            };

            void Compress(void* stream);
            void Decompress(void* stream);

            ~Huffman() {

            }
        
        private:
            void ComputeEntryDistribution();
        
    };

};