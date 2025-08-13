#pragma once

#include "../fs/fileStream.hpp"
#include "../fs/path.hpp"
#include "../logger.hpp"
#include "header.hpp"
#include <vector>

namespace audio {

class Player {
	bool initialized;
	bool playing;
	AudioFormat format;
	fs::FileStream stream;

	float currentSeconds;
	float totalSeconds;

	header_t header;
	std::vector<uint16_t> chunk;

public:
	Player(const fs::Path &file);

	bool output();

	void play();

	void pause();

	bool finished() const;

	/**
	 * @brief Seek to a specific time in the audio file.
	 * @param seconds The time in seconds to seek to.
	 */
	void seek(float seconds);

	/**
	 * @brief Get the current playback progress as a percentage.
	 * @return The current playback progress as a percentage (0.0 to 100.0).
	 */
	float progress();

	/**
	 * @brief Get the current playback position in seconds.
	 * @return The current playback position in seconds.
	 */
	float seconds();

	/**
	 * @brief Get the total duration of the audio file in seconds.
	 * @return The total duration of the audio file in seconds.
	 */
	float duration();

	bool good() const;
};

} // namespace audio