#ifndef COMPRESSOR_H 
#define COMPRESSOR_H

#include <stdint.h>

class Compressor {
   public:
      
      // return a number of bytes compressed
      virtual int32_t Compress(uint8_t *buffer, int32_t size) = 0;

      // return a number of bytes decompressed
      virtual int32_t Decompress(uint8_t *buffer) = 0;

      virtual ~Compressor() {};
   
};

#endif