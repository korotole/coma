#ifndef DATATYPES_H 
#define DATATYPES_H 

enum class COMPRESSION {
    RLE
};

enum RETURN_VALUES {
    SUCCESS = 0,
    E_OPEN,
    E_STAT,
    E_READ,
    E_WRITE,
    E_ALLOC
};

#endif
