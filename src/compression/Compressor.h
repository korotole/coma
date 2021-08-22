
class Compressor {
   public:
     
      virtual void Compress(void *stream) = 0;
      virtual void Decompress(void *stream) = 0;
   
};