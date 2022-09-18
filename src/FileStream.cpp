#include "FileStream.h"
#include "CompressionManager.h"

ssize_t FileStream::Read(uint8_t *tmp, size_t len)
{
    ssize_t ret = 0;

    while ((ret = pread(fd, tmp, (len > BUF_SIZE) ? BUF_SIZE : len, fileOffset)) != 0 && (len - ret >= 0)) {
        if (ret == -1) {
            if (errno == EINTR) continue; // handling some frequent interruptions
            return -1;
        }
        fileOffset += ret;
        len -= (int) ret;
    }

    return ret;
}

ssize_t FileStream::Write(const uint8_t *tmp, size_t len)
{
    ssize_t ret = 0;

    while ((ret = pwrite(fd, tmp, (len > BUF_SIZE) ? BUF_SIZE : len, fileOffset)) != 0 && (len - ret >= 0)) {
        if (ret == -1) {
            if (errno == EINTR) continue; // handling some frequent interruptions
            return -1;
        }
        fileOffset += ret;
        len -= (int) ret;
    }

    return ret;
}