#pragma once

#include <Arduino_AdvancedAnalog.h>

#include "fs.hpp"

namespace audio
{
    static AdvancedDAC dac0(A12);

    struct wav_header_t
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

    struct chunk_t
    {
        char ID[4];         // Chunk ID
        unsigned long size; // Chunk size
    };

    const std::vector<const char *> supportedTypes = {
        "wav",
    };

    bool supported(const String &ext)
    {
        return std::find(supportedTypes.begin(), supportedTypes.end(), ext) != supportedTypes.end();
    }

    class Player
    {
        fs::Path path;
        bool initialized;
        wav_header_t header;
        fs::FileStream stream;
        size_t totalSize;

    public:
        bool playing = false;

        Player(const String &filePath) : path(filePath), initialized(false), stream(path.stream()), playing(false)
        {
            auto ext = path.ext();

            if (ext != "wav")
            {
                logger::error("Unsupported file type: " + ext);
                return;
            }

            if (ext == "wav")
            {
                // Read WAV header
                auto headerData = stream.read<uint8_t>(sizeof(header));
                if (headerData.size() < sizeof(header))
                {
                    logger::error("Failed to read WAV header.");
                    return;
                }

                memcpy(&header, headerData.data(), sizeof(header));

                /* Find data chunk. */
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

                /* Determine number of samples. */
                int sample_size = header.bitsPerSample / 8;
                int samples_count = chunk.size * 8 / header.bitsPerSample;

                /* Configure the advanced DAC. */
                if (!dac0.begin(AN_RESOLUTION_12, header.sampleRate * 2, 256, 16))
                {
                    logger::error("Failed to start DAC0!");
                    return;
                }

                logger::info("WAV Player initialized for file: " + ext);
            }

            initialized = true;
            totalSize = stream.size();
        }

        bool output()
        {
            if (!playing)
            {
                return false;
            }

            if (!initialized || !dac0.available())
            {
                return false;
            }

            auto chunk = stream.read<uint16_t>(256);
            if (chunk.empty())
            {
                initialized = false; // Reset if no data is available
                return false;
            }

            /* Write data to buffer. */
            SampleBuffer buf = dac0.dequeue();
            for (size_t i = 0; i < buf.size(); i++)
            {
                /* Scale down to 12 bit. */
                uint16_t const dac_val = ((static_cast<unsigned int>(chunk[i]) + 32768) >> 4) & 0x0fff;
                buf[i] = dac_val;
            }

            /* Write the buffer to DAC. */
            dac0.write(buf);
        }

        void play()
        {
            playing = true;
        }

        void pause()
        {
            playing = false;
        }

        bool finished() const
        {
            return !initialized;
        }

        void seek(float seconds)
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return;
            }

            // Calculate the byte offset based on the sample rate and bits per sample
            unsigned long offset = static_cast<unsigned long>(seconds * header.sampleRate * (header.bitsPerSample / 8)) / 2.0f;
            stream.seek(offset, SEEK_SET);
        }

        float progress()
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return 0.0f;
            }

            // Calculate the current playback position as a percentage
            return (static_cast<float>(stream.tell()) / totalSize) * duration();
        }

        float duration()
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return 0.0f;
            }

            // Calculate the total duration in seconds
            return static_cast<float>(totalSize) / (header.sampleRate * (header.bitsPerSample / 8)) / 2.0f;
        }

        bool good() const
        {
            return initialized;
        }
    };
}
