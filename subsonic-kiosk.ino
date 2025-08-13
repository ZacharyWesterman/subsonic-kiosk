#include "src/audio.hpp"
#include "src/callback.hpp"
#include "src/fs.hpp"
#include "src/net.hpp"
#include "src/pins.hpp"
#include "src/require.hpp"
#include "src/uid.hpp"

audio::Player *player;

callback::repeat *progress = nullptr;

void setup() {
	pins::init();

#ifdef DEBUG
	require::serial(); // Initialize serial communication
#endif

	fs::connect(); // Ensure filesystem is connected

	// Initialize the audio player with a specific file, if supported.
	fs::Path filename("/spark.wav");
	if (filename.isFile() && audio::supported(filename.ext())) {
		player = new audio::Player(filename);
		player->play();

		progress = new callback::repeat(1000, []() { logger::info("Current playback progress: " + String(player->seconds()) + "/" + String(player->duration()) + " seconds"); });
	}
}

void loop() {
	if (!fs::connected()) {
		logger::warn("USB device disconnected! Attempting to reconnect...");
		fs::connect();

		return;
	}

	if (player && player->good()) {
		(*progress)();

		if (player->finished()) {
			logger::info("Playback finished.");
			delete player;
			player = nullptr;
		} else {
			player->output();
		}
	}
}
