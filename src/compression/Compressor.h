#ifndef COMPRESSOR_H 
#define COMPRESSOR_H 

class Compressor {
   public:
     
      virtual void Compress(void *stream) = 0;
      virtual void Decompress(void *stream) = 0;

      virtual ~Compressor() {};
   
};

#endif