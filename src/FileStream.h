#pragma once

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

class FileStream 
{
  private:
    int fd;
    int64_t fileOffset = 0;
  
  public:
    size_t size;

    FileStream() { }

    FileStream(int fd, size_t size) : fd(fd), size(size)
    {
    }

    ~FileStream() { 
      close(fd);
    }

    ssize_t Read(uint8_t *tmp, size_t len);
    ssize_t Write(const uint8_t *tmp, size_t len);
};