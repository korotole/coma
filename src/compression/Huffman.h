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
#include "../../conf/conf.h"
#include "../datatypes.h"
#include "../../conf/conf.h"
#include "Compressor.h"

namespace huffman {

    typedef struct Nodes {
        uint8_t key;
        uint32_t value;
        struct Nodes* left;
        struct Nodes* right;
    } Node;

    struct CompareObj {
        bool operator()(Node* l, Node* r) const {
            return l->value != r->value ? l->value < r->value : l->key < r->key;
        }
    };
 
    class Huffman : public Compressor {
        private:
            std::map<uint8_t, uint32_t> distribution = {};
            std::list<Node*> nodes = {};
            std::vector<bool> code = {};
            std::map<uint8_t, std::vector<bool>> table = {};
            int32_t fd;
        
        
        public:

            Huffman(int32_t fd) {
                this->fd = fd;
            };

            void Compress(void* stream);
            void Decompress(void* stream);

            ~Huffman() {
                FreeNodes(nodes.front());
            }
        
        private:
            uint8_t ComputeDistribution(int32_t fd);
            void AchieveNodes(std::map<uint8_t, uint32_t> distribution);
            bool CheckMap(std::map<uint8_t, uint32_t> m, uint8_t key);
            void SortAndMerge(std::list<Node*> nodes);
            void FreeNodes(Node *node);
        
    };

};

#endif