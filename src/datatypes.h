#ifndef DATATYPES_H 
#define DATATYPES_H 

#include <stdint.h>

enum class COMPRESSION {
    HUFFMAN
};

enum RETURN_VALUES {
    SUCCESS = 0,
    E_OPEN,
    E_STAT,
    E_READ,
    E_ALLOC
};


#endif