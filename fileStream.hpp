#pragma once

#include <vector>

#include "logger.hpp"

namespace fs
{
    class FileStream
    {
        FILE *file;

    public:
        FileStream(FILE *file = nullptr) : file(file) {}

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
            return true;
        }

        size_t tell() const
        {
            return ftell(file);
        }

        size_t size() const
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return 0;
            }

            auto currentPos = ftell(file);
            fseek(file, 0, SEEK_END);
            auto fileSize = ftell(file);
            fseek(file, currentPos, SEEK_SET);
            return fileSize;
        }
    };
}