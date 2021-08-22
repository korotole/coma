#include <stdint.h>

enum class COMPRESSION {
    HUFFMAN
};

enum RETURN_VALUES {
    SUCCESS = 0,
    OPEN,
    STAT,
    READ,
    ALLOCATION
};