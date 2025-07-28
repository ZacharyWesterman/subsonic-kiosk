#pragma once

#include <vector>

#include "logger.hpp"

namespace fs
{
    class FileStream
    {
        FILE *file;
        size_t index;

    public:
        FileStream(FILE *file = nullptr) : file(file), index(0) {}

        ~FileStream()
        {
            if (file)
            {
                fclose(file);
            }
        }

        std::vector<uint8_t> read(int chunkSize = 1024)
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return {};
            }

            std::vector<uint8_t> buffer(chunkSize);
            size_t bytesRead = fread(buffer.data(), 1, chunkSize, file);
            if (bytesRead == 0 && ferror(file))
            {
                logger::error("Error reading from file.");
                return {};
            }

            buffer.resize(bytesRead);
            return buffer;
        }

        bool seek(size_t position)
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return false;
            }

            if (fseek(file, position, SEEK_SET) != 0)
            {
                logger::error("Failed to seek in file.");
                return false;
            }
            index = position;
            return true;
        }
    };
}