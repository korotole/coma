#include "CompressionManager.h"

uint32_t CompressionManager::compress(){
    openFile();
    printf("File was successfully opened\n");
    return 0;
}

void CompressionManager::openFile(){

    struct stat sb;
    if ( stat(this->file, &sb ) == -1) {
        perror("stat");
        exit(STAT);
    }

    this->contents = calloc(1, sb.st_size);

    int32_t fd = open64(this->file, O_RDONLY);
    int64_t res = read(fd, contents, sb.st_size);
    if( res < 0 ) {
        perror("pread64");
        exit(READ);
    }

    close(fd);

    // for(uint8_t i = 0; i < strlen((char*)this->contents); i ++){
    //     uint8_t* co = (uint8_t*)this->contents;
    //     printf("%u\n", co[i]);
    // }

    return;
}
