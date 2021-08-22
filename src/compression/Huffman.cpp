#include "Huffman.h"

using namespace huffman;

void Huffman::Compress(void* stream) {

    std::cout << "Compression from Huffman!!" << std::endl;

}

void Huffman::Decompress(void* stream) {

}

void Huffman::ComputeEntryDistribution() {
    //     int64_t res = read(fd, this->fstream, this->fstreamSize + 1);
    // // int64_t res = fread(fstream, sb.st_size+1, 1, fd);

    // if( res < 0 ) {
    //     perror("pread64");
    //     exit(READ);
    // }

    // // printf("%s", this->contents);

    // for(uint32_t i = 0; i < sb.st_size+1; i++) {
    //      printf("%u:\t" PRINTF_BINARY_PATTERN_INT16 "\n", i, PRINTF_BYTE_TO_BINARY_INT16((int)this->fstream[i]));
    //}
}