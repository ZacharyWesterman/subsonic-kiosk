#include "header.hpp"

namespace audio {

namespace wav {

bool valid(const header_t &header) {
	return (*(unsigned int *)header.chunkID == 0x46464952) && (*(unsigned int *)header.format == 0x45564157);
}

void seekData(fs::FileStream &stream) {
	stream.seek(44); // Skip the header to the data chunk
}

unsigned long sampleRate(const header_t &header) {
	return header.sampleRate;
}

} // namespace wav

header_t::header_t() {}

bool validHeader(const header_t &header) {
	return wav::valid(header.wav);
}

std::vector<uint16_t> getChunk(fs::FileStream &stream, int chunkSize, AudioFormat format) {
	if (format == WAV) {
		/* Read the data chunk. */
		return stream.read<uint16_t>(chunkSize);
	} else {
		logger::error("Unsupported audio format for chunk reading.");
		return {};
	}
}

float getCurrentSeconds(const fs::FileStream &stream, const header_t &header, AudioFormat format) {
	if (format == WAV) {
		unsigned long position = stream.tell() - 44;
		return static_cast<float>(position) / header.wav.byteRate;
	} else {
		logger::error("Unsupported audio format for current seconds calculation.");
		return 0.0f;
	}
}

float getTotalSeconds(const fs::FileStream &stream, const header_t &header, AudioFormat format) {
	if (format == WAV) {
		unsigned long totalSize = stream.size() - 44;
		return static_cast<float>(totalSize) / header.wav.byteRate;
	} else {
		logger::error("Unsupported audio format for total seconds calculation.");
		return 0.0f;
	}
}

} // namespace audio
