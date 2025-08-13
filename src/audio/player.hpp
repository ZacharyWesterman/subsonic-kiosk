#pragma once

#include "../fs/fileStream.hpp"
#include "../fs/path.hpp"
#include "../logger.hpp"
#include "header.hpp"
#include <vector>

namespace audio {

/**
 * @brief A class to represent an audio player.
 * This class provides methods to play, pause, and control audio playback.
 */
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
	/**
	 * @brief Constructor for the Player class.
	 * @param file The file path to the audio file to play.
	 */
	Player(const fs::Path &file);

	/**
	 * @brief Output the audio data to the audio device.
	 * @return True if the output was successful, false otherwise (invalid audio or playback is finished).
	 */
	bool output();

	/**
	 * @brief Play or continue the audio file.
	 */
	void play();

	/**
	 * @brief Pause the audio playback until resumed.
	 */
	void pause();

	/**
	 * @brief Check if the audio has finished playing.
	 * @return True if the audio has finished playing, false otherwise.
	 */
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

	/**
	 * @brief Check if the audio player is in a good state.
	 * @return True if the audio player is good, false otherwise.
	 */
	bool good() const;
};

} // namespace audio
