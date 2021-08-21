#include <stdint.h>

typedef struct node {
    uint8_t a;
    void* left;
    void* right;
} Node;

enum class COMPRESSION {
    HUFFMAN
};

enum RETURN_VALUES {
    SUCCESS = 0,
    STAT,
    READ
};