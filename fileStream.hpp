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

        template <typename T>
        std::vector<T> read(int chunkSize = 1024)
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return {};
            }

            std::vector<T> buffer(chunkSize);
            size_t dataRead = fread(buffer.data(), sizeof(T), chunkSize, file);
            if (dataRead == 0 && ferror(file))
            {
                logger::error("Error reading from file.");
                return {};
            }

            buffer.resize(dataRead);
            return buffer;
        }

        bool seek(size_t position, int flag = SEEK_SET)
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return false;
            }

            if (fseek(file, position, flag) != 0)
            {
                logger::error("Failed to seek in file.");
                return false;
            }
            index = position;
            return true;
        }
    };
}