#include "FileStream.h"
#include "CompressionManager.h"

#include <iostream>

ssize_t FileStream::Read(uint8_t *tmp, size_t len)
{
    ssize_t ret = 0;
    ssize_t readBytes = 0;

    while ((ret = pread(fd, tmp, (len > BUF_SIZE) ? BUF_SIZE : len, fileOffset)) != 0 && (len - ret >= 0)) {
        if (ret == -1) {
            if (errno == EINTR) continue; // handling some frequent interruptions
            return -1;
        }
        fileOffset += ret;
        len -= (int) ret;
        readBytes += ret;
    }

    return readBytes;
}

ssize_t FileStream::Write(const uint8_t *tmp, size_t len)
{
    ssize_t ret = 0;
    ssize_t writtenBytes = 0;
    
    while ((ret = pwrite(fd, tmp, (len > BUF_SIZE) ? BUF_SIZE : len, fileOffset)) != 0 && (len - ret >= 0)) {
        if (ret == -1) {
            if (errno == EINTR) continue; // handling some frequent interruptions
            return -1;
        }
        fileOffset += ret;
        len -= (int) ret;
        writtenBytes += ret;
    }

    return writtenBytes;
}