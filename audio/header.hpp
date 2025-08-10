#pragma once

#include <vector>

namespace audio
{
    struct header_base_t
    {
        virtual bool valid() const = 0;
        virtual void seekData(fs::FileStream &stream) const = 0;
        virtual unsigned long getSampleRate() const = 0;

        /**
         * @brief Read a chunk of data from the audio file, decoding it into raw signal data.
         * @param stream The file stream to read from.
         * @param chunkSize The minimum size of the chunk to read.
         * @return A vector containing raw signal data.
         */
        virtual std::vector<uint8_t> getChunk(fs::FileStream &stream, int chunkSize) const = 0;
    };

    namespace wav
    {
        struct chunk_t
        {
            char ID[4];         // Chunk ID
            unsigned long size; // Chunk size
        };

        struct header_t : public header_base_t
        {
            char chunkID[4];             //"RIFF" = 0x46464952
            unsigned long chunkSize;     // 28 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes] + sum(sizeof(chunk.id) + sizeof(chunk.size) + chunk.size)
            char format[4];              //"WAVE" = 0x45564157
            char subchunk1ID[4];         //"fmt " = 0x20746D66
            unsigned long subchunk1Size; // 16 [+ sizeof(wExtraFormatBytes) + wExtraFormatBytes]
            unsigned short audioFormat;
            unsigned short numChannels;
            unsigned long sampleRate;
            unsigned long byteRate;
            unsigned short blockAlign;
            unsigned short bitsPerSample;

            virtual bool valid() const override
            {
                return (chunkID[0] == 0x46 &&
                        chunkID[1] == 0x46 &&
                        chunkID[2] == 0x49 &&
                        chunkID[3] == 0x52) &&
                       (format[0] == 0x45 &&
                        format[1] == 0x56 &&
                        format[2] == 0x41 &&
                        format[3] == 0x57);
            }

            virtual void seekData(fs::FileStream &stream) const override
            {
                /* Find first data chunk. */
                chunk_t chunk;
                while (true)
                {
                    auto chunkData = stream.read<uint8_t>(sizeof(chunk));
                    memcpy(&chunk, chunkData.data(), sizeof(chunk));

                    if (*(unsigned int *)&chunk.ID == 0x61746164)
                        break;

                    /* Skip chunk data bytes. */
                    stream.seek(chunk.size, SEEK_CUR);
                }
            }

            virtual unsigned long getSampleRate() const override
            {
                return sampleRate;
            }

            virtual std::vector<uint8_t> getChunk(fs::FileStream &stream, int chunkSize) const override
            {
                /* Read the data chunk. */
                return stream.read<uint8_t>(chunkSize);
            }
        };
    }

    union header_t
    {
        wav::header_t wav;
        // Add other formats here if needed

        header_t() {}
    };

    enum AudioFormat
    {
        NO_AUDIO,
        WAV,
        // Add other formats here if needed
    };

    bool validHeader(const header_t &header)
    {
        return header.wav.valid();
    }
}