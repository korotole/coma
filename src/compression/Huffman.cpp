#include "Huffman.h"

using namespace huffman;

void Huffman::Compress(void* stream) {

    std::cout << "Compression from Huffman!!" << std::endl;
    ComputeEntryDistribution(this->fd);

}

void Huffman::Decompress(void* stream) {

}

int32_t Huffman::ComputeEntryDistribution(int32_t fd) {

    ssize_t ret = 0;
    uint8_t buf = 0;
    uint64_t fileOffset1 = 0;

    uint64_t mapCounter = 0;
    
    while((ret = pread(fd, &buf, 1, fileOffset1)) != 0){
        if(ret == -1){
            if (errno == EINTR) continue; // handling some frequent interruptions
            return E_READ;
        }
        fileOffset1++;

        if(!CheckMap(this->distribution, buf)) {
            distribution.emplace(buf, 1);
        } else {
            distribution[buf] = ++distribution[buf];
        }

    }

    std::cout << std::endl;
    for( auto it = this->distribution.cbegin(); it != this->distribution.cend(); it++ ){

        printf(PRINTF_BINARY_PATTERN_INT64 "\t%d\n", PRINTF_BYTE_TO_BINARY_INT64((*it).first), (*it).second);
        // std::cout << (*it).first << "\t" << (*it).second << std::endl;
    }

    std::cout << "\nSize of map:\t" << this->distribution.size() << std::endl;
    std::cout << "Size of file:\t" << fileOffset1 << std::endl;

    return SUCCESS;
}

bool Huffman::CheckMap(std::map<uint8_t, uint32_t> m, uint8_t key) {

    return (m.find(key) == m.end()) ? false : true;
}

    // for(uint32_t i = 0; i < sb.st_size+1; i++) {
    //      printf("%u:\t" PRINTF_BINARY_PATTERN_INT16 "\n", i, PRINTF_BYTE_TO_BINARY_INT16((int)this->fstream[i]));
    //}


/*
size_t slow_copy(int fd1, int fd2, off_t fileOffset1, off_t fileOffset2, int amount){
    char buf;
    ssize_t ret;
    
    while((ret = pread(fd1, &buf, 1, fileOffset1)) != 0 && (amount == WHOLEFILE || amount > 0)){
        if(ret == -1){
            if (errno == EINTR) continue; // handling some frequent interruptions
            return E_READ;
        }
        fileOffset1++;
        if((ret = pwrite(fd2, &buf, 1, fileOffset2)) != 0){
            if(ret == -1){
                if (errno == EINTR) continue; // handling some frequent interruptions
                return E_WRITE;
            }
        }
        if(amount != WHOLEFILE) amount -= (int) ret;
        fileOffset2++;
    }
    return SUCCESS;
}
*/