#pragma once

#include "../FileStream.h"
#include "../../conf/conf.h"
#include <memory>
#include <string.h>

static struct singleCompression
{
    uint8_t data[3] = {0};       // metadata read from buffer (flag, length, value), describes single compression
    uint8_t *buf = nullptr;      // buffer that is being dynamically allocated and filled with decoded symbols
    size_t length = 0;           // number of repetitions of the very value (length of decompressed string produced)
    int size = 0;                // number of bytes for current read (metadata + data), number of data[] fields used
    int rawBytesToBeWritten = 0; // number of NON-REPEAT bytes that are to be written directly at the next buffer read (overflow)
    int readFromPrevBuffer = 0;  // number of data[] fields read from previous buffer
} SingleDecompression;

template <size_t BufferSize=BUF_SIZE>
class RLE
{
  public:
    FileStream *source;
    FileStream *destination;

    static constexpr auto Max6bNumber = 0x3F;
    static constexpr auto Max5bNumber = 0x1F;

    static constexpr auto CompressionFlag = 0x80;
    static constexpr auto CompressedFlag = 0x80;
    static constexpr auto NotCompressedFlag = 0x00;

    static constexpr auto LengthFlag = 0x40;
    static constexpr auto SingleByteLengthFlag = 0x40;
    static constexpr auto TwoByteLengthFlag = 0x00;

    static constexpr auto ZeroByteFlag = 0x20;
    static constexpr auto ZeroByteData = 0x20;
    static constexpr auto MinimalLengthToInterrupt = 5;

  public:
    constexpr RLE(FileStream* source, FileStream* destination)
        : source(source), destination(destination)
    {
    }

    ssize_t Compress() const
    {
        auto buffer = std::make_unique<uint8_t[]>(BufferSize);
        ssize_t total_length = 0;

        while (total_length < source->size)
        {
            ssize_t read = source->Read(buffer.get(), BufferSize);
            if (read == 0)
            {
                break;
            }

            compress_buffer(buffer.get(), read);
            total_length += read;
        }

        return total_length;
    }

    ssize_t Decompress()
    {
        auto buffer = std::make_unique<uint8_t[]>(BufferSize);
        ssize_t total_length = 0;

        while (total_length < source->size)
        {
            int read = source->Read(buffer.get(), BufferSize);

            if (read == 0)
            {
                break;
            }

            decompress_buffer(buffer.get(), read);
            total_length += read;
        }

        return total_length;
    }

    static ssize_t Compression(FileStream *src, FileStream *dest)
    {
        return RLE(src, dest).Compress();
    }

    static ssize_t Decompression(FileStream *src, FileStream *dest)
    {
        return RLE(src, dest).Decompress();
    }

  private:
    size_t decode_length(uint8_t first, uint8_t second) const
    {
        size_t length = 0;
        if ((first & CompressionFlag) == CompressedFlag)
        {
            if ((first & LengthFlag) == SingleByteLengthFlag)
            {
                length = (first & Max6bNumber);
                length += 2;
            }
            else // TwoByteLengthFlag
            {
                length = ((first & Max5bNumber) << 8);
                length += (((Max6bNumber + 1) + 2) + second);
            }
        }
        else // NotCompressedFlag
        {
            if ((first & LengthFlag) == SingleByteLengthFlag)
            {
                length = (first & Max6bNumber);
                length += 1;
            }
            else // TwoByteLengthFlag
            {
                length = ((first & Max6bNumber) << 8);
                length += (((Max6bNumber + 1) + 1) + second);
            }
        }

        return length;
    }

    void write_repeat(int length, uint8_t byte) const
    {
        uint8_t data[3] = {0};
        length -= 2; // repeat value must be at least 2, otherwise it is not repeat
        if (length <= Max6bNumber)
        {
            data[0] = (CompressedFlag | SingleByteLengthFlag | (uint8_t)(length & Max6bNumber));
            data[1] = byte;
            destination->Write(data, 2);
            return;
        }

        length -= (Max6bNumber + 1);
        data[0] = (CompressedFlag | TwoByteLengthFlag | (uint8_t)((length >> 8) & Max5bNumber));
        data[1] = (uint8_t)(length);
        if (byte == 0)
        {
            data[0] |= ZeroByteData;
            destination->Write(data, 2);
            return;
        }

        data[2] = byte;
        destination->Write(data, 3);
    }

    void write_non_repeat_buffer(const uint8_t *buffer, int data_length) const
    {
        uint8_t data[2] = {0};
        int written_length = (data_length - 1); // count value must be at least 1
        if (written_length <= Max6bNumber)
        {
            data[0] = (NotCompressedFlag | SingleByteLengthFlag |
                       (uint8_t)(written_length & Max6bNumber));
            destination->Write(data, 1);
        }
        else
        {
            written_length -= (Max6bNumber + 1);
            data[0] = (NotCompressedFlag | TwoByteLengthFlag |
                       (uint8_t)((written_length >> 8) & Max6bNumber));
            data[1] = (uint8_t)(written_length);
            destination->Write(data, 2);
        }

        destination->Write(buffer, data_length);
    }

    int write_non_repeat(const uint8_t *buffer, int size) const
    {
        int non_repeat_length = 0;
        const uint8_t *position = buffer;
        while (non_repeat_length < size)
        {
            int repeat_length = get_repeat_length(position, size - non_repeat_length);
            if (repeat_length > MinimalLengthToInterrupt)
            {
                break;
            }

            position++;
            non_repeat_length++;
        }

        if (non_repeat_length > 0)
        {
            write_non_repeat_buffer(buffer, non_repeat_length);
        }

        return (non_repeat_length);
    }

    void compress_buffer(const uint8_t *buffer, int size) const
    {
        while (size > 0)
        {
            int length = get_repeat_length(buffer, size);
            if (length > 3)
            {
                write_repeat(length, *buffer);
            }
            else
            {
                length = write_non_repeat(buffer, size);
            }

            buffer += length;
            size -= length;
        }
    }

    int get_repeat_length(const uint8_t *buffer, int size) const
    {
        int count = 0;
        uint8_t start = *buffer;
        while ((size > 0) && (*buffer == start))
        {
            size--;
            buffer++;
            count++;
        }

        return (count);
    }

    /**
     * @brief determine the size of compression (size of data[] fields to be used) 
     * by parsing the First byte of single compression info
     */
    void parse_fbyte(const uint8_t *buffer, int *size)
    {
        SingleDecompression.data[0] = *buffer;
        *size -= 1;
        SingleDecompression.readFromPrevBuffer++;

        if (((SingleDecompression.data[0] & LengthFlag) == SingleByteLengthFlag ||
             (SingleDecompression.data[0] & ZeroByteFlag) == ZeroByteFlag) &&
             (SingleDecompression.data[0] & CompressionFlag) == CompressedFlag)
        {
            SingleDecompression.size = 2;
        }
        else if ((SingleDecompression.data[0] & CompressionFlag) == NotCompressedFlag &&
                 (SingleDecompression.data[0] & LengthFlag) == TwoByteLengthFlag)
        {
            SingleDecompression.size = 2;
        }
        else if ((SingleDecompression.data[0] & CompressionFlag) == NotCompressedFlag &&
                 (SingleDecompression.data[0] & LengthFlag) == SingleByteLengthFlag)
        {
            SingleDecompression.size = 1;
        }
        else // CompressedFlag, 2bLength, non-zero
        {
            SingleDecompression.size = 3;
        }
    }

    /**
     * @brief Initialize a single decompression, by allocating buf
     * of the proper structure and filling it with decompressed data
     * @return 0 or the length of non-repeat data stored at buf
     */
    size_t init_single_decompression(uint8_t *buffer, int *size)
    {
        size_t res = 0;
        SingleDecompression.length = decode_length(SingleDecompression.data[0],
                                                  (SingleDecompression.size == 1) ? 0 : SingleDecompression.data[1]);                                

        // set the size of dynamically allocated buffer of decompressed symbols
        if (*size >= SingleDecompression.length ||
           (*size < SingleDecompression.length && ((SingleDecompression.data[0] & CompressionFlag) == CompressedFlag)))
        {         
            SingleDecompression.buf = (uint8_t *)calloc(SingleDecompression.length, sizeof(uint8_t));
        }
        else
        {
            SingleDecompression.buf = (uint8_t *)calloc(*size, sizeof(uint8_t));
        }

        // put the decompressed symbols into a buffer
        if ((SingleDecompression.data[0] & LengthFlag) == SingleByteLengthFlag &&
            (SingleDecompression.data[0] & CompressionFlag) == CompressedFlag)
        {
            memset(SingleDecompression.buf, SingleDecompression.data[1], SingleDecompression.length);
        }
        else if ((SingleDecompression.data[0] & LengthFlag) == TwoByteLengthFlag &&
                 (SingleDecompression.data[0] & ZeroByteFlag) != ZeroByteFlag &&
                 (SingleDecompression.data[0] & CompressionFlag) == CompressedFlag)
        {
            memset(SingleDecompression.buf, SingleDecompression.data[2], SingleDecompression.length);
        }
        else if ((SingleDecompression.data[0] & ZeroByteFlag) == ZeroByteFlag &&
                 (SingleDecompression.data[0] & LengthFlag) == TwoByteLengthFlag &&
                 (SingleDecompression.data[0] & CompressionFlag) == CompressedFlag)
        {
            memset(SingleDecompression.buf, 0x00, SingleDecompression.length);
        }
        else // NON-REPEAT case
        {
            if (*size >= SingleDecompression.length)
            {    
                memcpy(SingleDecompression.buf, buffer, SingleDecompression.length);
                *size -= SingleDecompression.length;
                res = SingleDecompression.length;
            }
            else // a part of uncompressed symbols is in the next buffer (further read)
            {
                memcpy(SingleDecompression.buf, buffer, *size);
                SingleDecompression.readFromPrevBuffer = 0;
                SingleDecompression.rawBytesToBeWritten = SingleDecompression.length - *size;
                res = *size;
                *size = 0;
            }
        }

        return res;
    }

    void decompress_buffer(uint8_t *buffer, int size)
    {
        while (size > 0)
        {
            if (SingleDecompression.readFromPrevBuffer != 0) // data[] not filled completely 'cause prev buffer ended
            {
                memcpy(SingleDecompression.data + SingleDecompression.readFromPrevBuffer,
                       buffer,
                       SingleDecompression.size - SingleDecompression.readFromPrevBuffer);

                size -= (SingleDecompression.size - SingleDecompression.readFromPrevBuffer);
                buffer += (SingleDecompression.size - SingleDecompression.readFromPrevBuffer);
                SingleDecompression.readFromPrevBuffer = 0;
            }
            else if (SingleDecompression.rawBytesToBeWritten != 0) // uncompressed bytes (continuation, if prev buffer did not contain all of them)
            {
                int currentWrite = SingleDecompression.rawBytesToBeWritten <= size ? SingleDecompression.rawBytesToBeWritten : size;

                destination->Write(buffer, currentWrite);

                buffer += currentWrite;
                size -= currentWrite;

                if(currentWrite == SingleDecompression.rawBytesToBeWritten) 
                    memset(&SingleDecompression, 0, sizeof(SingleDecompression));
                else 
                    SingleDecompression.rawBytesToBeWritten -= currentWrite;

                continue;
            }
            else // usual case
            {
                parse_fbyte(buffer, &size);
                buffer += 1;

                if (size < SingleDecompression.size - SingleDecompression.readFromPrevBuffer) // if some data[] contents is in the next buffer
                {
                    memcpy(SingleDecompression.data + SingleDecompression.readFromPrevBuffer, buffer, size);
                    buffer += size;
                    SingleDecompression.readFromPrevBuffer += size;
                    size -= size; // must be equal to 0 to get the next buffer

                    continue;
                }
                else // usual case
                {
                    memcpy(SingleDecompression.data + SingleDecompression.readFromPrevBuffer,
                           buffer,
                           SingleDecompression.size - SingleDecompression.readFromPrevBuffer);

                    size -= (SingleDecompression.size - SingleDecompression.readFromPrevBuffer);
                    buffer += (SingleDecompression.size - SingleDecompression.readFromPrevBuffer);
                }
            }

            buffer += init_single_decompression(buffer, &size);

            if (SingleDecompression.length == 0)
            {
                memset(&SingleDecompression, 0, sizeof(SingleDecompression));
                return;
            }

            destination->Write(SingleDecompression.buf, (int) SingleDecompression.length - SingleDecompression.rawBytesToBeWritten);

            free(SingleDecompression.buf);

            if (SingleDecompression.rawBytesToBeWritten == 0)
                memset(&SingleDecompression, 0, sizeof(SingleDecompression));
        }
    }
};
