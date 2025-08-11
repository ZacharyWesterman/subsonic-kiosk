#pragma once

#include <vector>

namespace audio
{
    enum AudioFormat
    {
        NO_AUDIO,
        WAV,
        // Add other formats here if needed
    };

    namespace wav
    {
        struct header_t
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
        };

        bool valid(const header_t &header)
        {
            return (*(unsigned int *)header.chunkID == 0x46464952) &&
                   (*(unsigned int *)header.format == 0x45564157);
        }

        void seekData(fs::FileStream &stream)
        {
            stream.seek(44); // Skip the header to the data chunk
        }

        unsigned long sampleRate(const header_t &header)
        {
            return header.sampleRate;
        }
    }

    union header_t
    {
        wav::header_t wav;
        // Add other formats here if needed

        header_t() {}
    };

    bool validHeader(const header_t &header)
    {
        return wav::valid(header.wav);
    }

    /**
     * @brief Read a chunk of data from the audio file, decoding it into raw signal data.
     * @param stream The file stream to read from.
     * @param chunkSize The minimum size of the chunk to read.
     * @param format The audio format of the file.
     * @return A vector containing raw signal data.
     */
    std::vector<uint16_t> getChunk(fs::FileStream &stream, int chunkSize, AudioFormat format)
    {
        if (format == WAV)
        {
            /* Read the data chunk. */
            return stream.read<uint16_t>(chunkSize);
        }
        else
        {
            logger::error("Unsupported audio format for chunk reading.");
            return {};
        }
    }

    /**
     * @brief Get the current playback time in seconds.
     * @param stream The file stream to read from.
     * @param header The audio file header.
     * @param format The audio format of the file.
     * @return The current playback time in seconds.
     */
    float getCurrentSeconds(const fs::FileStream &stream, const header_t &header, AudioFormat format)
    {
        if (format == WAV)
        {
            unsigned long totalSize = stream.size() - 44;
            unsigned long position = stream.tell() - 44;

            return static_cast<float>(position) / totalSize * header.wav.sampleRate;
        }
        else
        {
            logger::error("Unsupported audio format for current seconds calculation.");
            return 0.0f;
        }
    }
}
