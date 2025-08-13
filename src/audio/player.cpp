#include "player.hpp"
#include <Arduino.h>
#include <Arduino_AdvancedAnalog.h>

namespace audio {

static AdvancedDAC dac0(A12);

Player::Player(const fs::Path &file) : initialized(false), playing(false), format(NO_AUDIO), stream(file.stream()), currentSeconds(0.0f) {
	if (!stream) {
		logger::error("Failed to open audio file stream.");
		return;
	}

	stream.seek(0);

	// Read file header to determine type
	auto headerData = stream.read<uint8_t>(sizeof(header));
	memcpy(&header, headerData.data(), headerData.size());

	if (!validHeader(header)) {
		logger::error("Invalid audio file header, cannot play.");
		return;
	}

	unsigned long sampleRate = -1;

	if (wav::valid(header.wav)) {
		format = WAV;
		wav::seekData(stream);
		sampleRate = wav::sampleRate(header.wav);
	} else {
		logger::error("Unsupported audio format.");
		return;
	}

	/* Configure the advanced DAC. */
	if (!dac0.begin(AN_RESOLUTION_12, sampleRate * 2, 256, 16)) {
		logger::error("Failed to start DAC0!");
		return;
	}

	totalSeconds = getTotalSeconds(stream, header, format);

	logger::info("Player initialized for file.");

	initialized = true;
}

bool Player::output() {
	if (!playing) {
		return false;
	}

	if (!initialized || !dac0.available()) {
		return false;
	}

	if (!format) {
		return false;
	}

	SampleBuffer buf = dac0.dequeue();

	// Get the next chunk of audio data if the current chunk is empty or too small.
	if (chunk.size() < buf.size()) {
		auto ch = getChunk(stream, buf.size(), format);
		chunk.insert(chunk.end(), ch.begin(), ch.end());
	}

	if (chunk.size() < buf.size()) {
		initialized = false; // Reset if no data is available
		return false;
	}

	// Write raw signal data to buffer.
	for (size_t i = 0; i < buf.size(); i++) {
		// Scale down to 12 bit.
		uint16_t const dac_val = ((static_cast<unsigned int>(chunk[i]) + 32768) >> 4) & 0x0fff;
		buf[i] = dac_val;
	}
	// Remove the processed data from the chunk.
	chunk.erase(chunk.begin(), chunk.begin() + buf.size());

	// Write the buffer to DAC.
	dac0.write(buf);

	currentSeconds = getCurrentSeconds(stream, header, format);
}

void Player::play() {
	playing = true;
}

void Player::pause() {
	playing = false;
}

bool Player::finished() const {
	return !initialized;
}

void Player::seek(float seconds) {
	if (!initialized) {
		logger::error("Player not initialized.");
		return;
	}

	// TODO: Implement seeking functionality
}

float Player::progress() {
	if (!initialized) {
		logger::error("Player not initialized.");
		return 0.0f;
	}

	return (currentSeconds / totalSeconds) * 100.0f;
}

float Player::seconds() {
	if (!initialized) {
		logger::error("Player not initialized.");
		return 0.0f;
	}

	return currentSeconds;
}

float Player::duration() {
	if (!initialized) {
		logger::error("Player not initialized.");
		return 0.0f;
	}

	return totalSeconds;
}

bool Player::good() const {
	return initialized;
}

} // namespace audio