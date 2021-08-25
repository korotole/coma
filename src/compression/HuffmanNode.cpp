#include "HuffmanNode.h"

bool HuffmanNode::Compare(HuffmanNode *L, HuffmanNode *R) const {

    // Compares two HuffmanNodes by their value (number of symbol occurances)
    return L->value != R->value ? L->value < R->value : L->key < R->key;
}