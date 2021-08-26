#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <map>
#include <iostream>
#include <list>
#include <vector>

#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "HuffmanNode.h"
#include "../../conf/conf.h"
#include "../datatypes.h"
#include "../../conf/conf.h"
#include "Compressor.h"

class Huffman : public Compressor {

    private:
        std::map<uint8_t, uint32_t> distribution = {};
        std::list<HuffmanNode*> tree = {};
        std::vector<bool> code = {};
        std::map< uint8_t, std::vector<bool> > table = {};
        int32_t fd;
        
    public:

        Huffman(int32_t fd) {
            this->fd = fd;
        };

        void Compress(void* stream);
        void Decompress(void* stream);

        ~Huffman() {
            FreeNodes(this->tree.front());
        }
        
    private:
        uint8_t ComputeDistribution(int32_t fd);
        void AchieveNodes(void);
        bool CheckMap(std::map<uint8_t, uint32_t> m, uint8_t key);
        void SortAndMerge();
        void FreeNodes(HuffmanNode *node);
        void CreateCodeTable(HuffmanNode *root);
        uint8_t PrintTableElem();
        void PrintTable();
        
};


#endif