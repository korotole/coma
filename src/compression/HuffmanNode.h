#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

#include "stdint.h"

class HuffmanNode {

    public:
        uint8_t key;
        uint32_t value;
        HuffmanNode *left;
        HuffmanNode *right;

        HuffmanNode() {

        }

        HuffmanNode(uint8_t K, uint32_t V) {
            this->key = K;
            this->value = V;
            this->left = nullptr;
            this->right = nullptr;
        }

        HuffmanNode(HuffmanNode *L, HuffmanNode *R) {
          this->left = L;
          this->right = R;
          this->key = (uint8_t) NULL;
          this->value = L->value + R->value;
        }

        ~HuffmanNode() {
          
        }

        bool Compare(HuffmanNode *L, HuffmanNode *R) const;

};

#endif