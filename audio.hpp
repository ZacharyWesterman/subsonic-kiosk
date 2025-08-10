#pragma once

#include <Arduino_AdvancedAnalog.h>

#include "fs.hpp"
#include "audio/header.hpp"

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

    const std::vector<const char *> supportedTypes = {
        "wav",
    };

    bool supported(const String &ext)
    {
        return std::find(supportedTypes.begin(), supportedTypes.end(), ext) != supportedTypes.end();
    }

    class Player
    {
        bool initialized;
        bool playing;
        AudioFormat format;
        fs::FileStream stream;
        size_t totalSize;
        header_t header;
        std::vector<uint8_t> chunk;

    public:
        Player(fs::FileStream &&fileStream) : initialized(false), playing(false), format(NO_AUDIO), stream(fileStream)
        {
            if (!stream)
            {
                logger::error("Failed to open audio file stream.");
                return;
            }

            stream.seek(0);

            // Read file header to determine type
            auto headerData = stream.read<uint8_t>(sizeof(header));
            memcpy(&header, headerData.data(), headerData.size());

            if (!validHeader(header))
            {
                logger::error("Invalid audio file header, cannot play.");
                return;
            }

            unsigned long sampleRate = -1;

            if (wav::valid(header.wav))
            {
                format = WAV;
                wav::seekData(stream);
                sampleRate = wav::sampleRate(header.wav);
            }
            else
            {
                logger::error("Unsupported audio format.");
                return;
            }

            /* Configure the advanced DAC. */
            if (!dac0.begin(AN_RESOLUTION_12, sampleRate * 2, 256, 16))
            {
                logger::error("Failed to start DAC0!");
                return;
            }

            logger::info("Player initialized for file.");

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

            if (!format)
            {
                return false;
            }

            SampleBuffer buf = dac0.dequeue();

            // Get the next chunk of audio data if the current chunk is empty or too small.
            if (chunk.size() < buf.size())
            {
                auto ch = getChunk(stream, buf.size(), format);
                chunk.insert(chunk.end(), ch.begin(), ch.end());
            }

            if (chunk.size() < buf.size())
            {
                initialized = false; // Reset if no data is available
                return false;
            }

            // Write raw signal data to buffer.
            for (size_t i = 0; i < buf.size(); i++)
            {
                // Scale down to 12 bit.
                uint16_t const dac_val = ((static_cast<unsigned int>(chunk[i]) + 32768) >> 4) & 0x0fff;
                buf[i] = dac_val;
            }
            // Remove the processed data from the chunk.
            chunk.erase(chunk.begin(), chunk.begin() + buf.size());

            // Write the buffer to DAC.
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

        /**
         * @brief Seek to a specific time in the audio file.
         * @param seconds The time in seconds to seek to.
         */
        void seek(float seconds)
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return;
            }

            // TODO: Implement seeking functionality
        }

        /**
         * @brief Get the current playback progress as a percentage.
         * @return The current playback progress as a percentage (0.0 to 100.0).
         */
        float progress()
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return 0.0f;
            }

            // TODO: Calculate the current playback position as a percentage
            return 0.0f;
        }

        /**
         * @brief Get the total duration of the audio file in seconds.
         * @return The total duration of the audio file in seconds.
         */
        float duration()
        {
            if (!initialized)
            {
                logger::error("Player not initialized.");
                return 0.0f;
            }

            // TODO: Calculate the total duration in seconds
            return 0.0f;
        }

        bool good() const
        {
            return initialized;
        }
    };
}
