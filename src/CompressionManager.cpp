#include "CompressionManager.h"
#include "../conf/conf.h"


uint8_t CompressionManager::Initialize(char* filename) {
    this->fdnum = 1;
    this->fds = (int32_t*) calloc(this->fdnum, sizeof(int32_t));

    this->file = (char *) calloc(1, strlen(filename)+1);
    strcpy(this->file, filename);

    compressor = new huffman::Huffman();
}

uint32_t CompressionManager::Compress(){

    for(int32_t i = 0; i < this->fdnum; i++) {
        ManageFileStream(i);
    }

    this->compressor->Compress(NULL);

    for(int32_t i = 0; i < this->fdnum; i++) {
        ManageFileStream(i);
    }

    return 0;
}

uint8_t CompressionManager::ManageFileStream(int32_t fd){

    if( !this->fds[fd] ){

        // get the size of a file
        struct stat sb;
        if ( stat(this->file, &sb ) == -1) {
            perror("stat");
            return STAT;
        }

        // save the size of a file
        this->fsize = sb.st_size;

        // open the file
        this->fds[fd] = open64(this->file, O_RDONLY);
        if( this->fds[fd] == -1){
            perror("open");
            return OPEN;
        }

    } else {

        // close the file if non-empty fd is provided
        close(this->fds[fd]);

    } 
    
    return SUCCESS;
}