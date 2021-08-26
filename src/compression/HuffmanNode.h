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
          this->key = 0;
          this->value = L->value + R->value;
        }

        ~HuffmanNode() {
          
        }

        struct OCompare {
            bool operator()(HuffmanNode *L, HuffmanNode *R) const {
              return L->value != R->value ? L->value < R->value : L->key < R->key;
            }
        };
};

#endif