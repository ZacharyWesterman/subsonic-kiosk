/// @file header.hpp
#pragma once

#include "../fs/fileStream.hpp"
#include <vector>

namespace audio {

/**
 * @brief An enumeration of supported audio formats.
 */
enum AudioFormat {
	NO_AUDIO,
	WAV,
	// Add other formats here if needed
};

namespace wav {

/**
 * @brief A structure to represent the header of a WAV audio file.
 */
struct header_t {
	/// The chunk ID. This should be "RIFF".
	char chunkID[4];
	/// The size of the chunk.
	unsigned long chunkSize;
	/// The format of the file. This should be "WAVE".
	char format[4];
	/// The subchunk ID. This should be "fmt ".
	char subchunk1ID[4];
	/// The size of the subchunk. Typically 16 for PCM.
	unsigned long subchunk1Size;
	/// The audio format. 1 for PCM.
	unsigned short audioFormat;
	/// The number of channels. 1 for mono, 2 for stereo.
	unsigned short numChannels;
	/// The sample rate in Hz.
	unsigned long sampleRate;
	/// The byte rate. This is sampleRate * numChannels * bitsPerSample/8.
	unsigned long byteRate;
	/// The block align.
	unsigned short blockAlign;
	/// The bits per sample.
	unsigned short bitsPerSample;
};

/**
 * @brief Check if the WAV header is valid.
 * @param header The WAV header to check.
 * @return True if the header is valid, false otherwise.
 */
bool valid(const header_t &header);

/**
 * @brief Seek to the data section of the WAV file.
 * @param stream The file stream to seek in.
 */
void seekData(fs::FileStream &stream);

/**
 * @brief Get the sample rate from the WAV header.
 * @param header The WAV header to get the sample rate from.
 * @return The sample rate.
 */
unsigned long sampleRate(const header_t &header);

} // namespace wav

/**
 * @brief A union to represent different audio headers.
 * This allows for reinterpretation of the header based on the audio format.
 */
union header_t {
	/// WAV header.
	wav::header_t wav;
	// Add other formats here if needed

	/**
	 * @brief Default constructor for the header_t union.
	 */
	header_t();
};

/**
 * @brief Check if the header data belongs to a supported audio format.
 * @param header The audio header to check.
 * @return True if the header is valid, false otherwise.
 */
bool validHeader(const header_t &header);

/**
 * @brief Read a chunk of data from the audio file, decoding it into raw signal data.
 * @param stream The file stream to read from.
 * @param chunkSize The minimum size of the chunk to read.
 * @param format The audio format of the file.
 * @return A vector containing raw signal data.
 */
std::vector<uint16_t> getChunk(fs::FileStream &stream, int chunkSize, AudioFormat format);

/**
 * @brief Get the current playback time in seconds.
 * @param stream The file stream to read from.
 * @param header The audio file header.
 * @param format The audio format of the file.
 * @return The current playback time in seconds.
 */
float getCurrentSeconds(const fs::FileStream &stream, const header_t &header, AudioFormat format);

/**
 * @brief Get the total duration of the audio file in seconds.
 * @param stream The file stream to read from.
 * @param header The audio file header.
 * @param format The audio format of the file.
 * @return The total duration of the audio file in seconds.
 */
float getTotalSeconds(const fs::FileStream &stream, const header_t &header, AudioFormat format);

} // namespace audio
