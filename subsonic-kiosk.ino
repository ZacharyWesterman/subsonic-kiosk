
#define SERIAL_DEBUG

#include "audio.hpp"
#include "callback.hpp"
#include "fs.hpp"
#include "net.hpp"
#include "pins.hpp"
#include "require.hpp"

audio::Player *player;
// int counter = 0;
bool NET_AVAILABLE = false;

callback::repeat *progress = nullptr;

void setup() {
	pins::init();

#ifdef SERIAL_DEBUG
	require::serial(); // Initialize serial communication
#endif

	fs::connect(); // Ensure filesystem is connected
	// request::netInit(); // Try to connect to the network, if available

	// Initialize the audio player with a specific file, if supported.
	fs::Path filename("/spark.wav");
	if (filename.isFile() && audio::supported(filename.ext())) {
		player = new audio::Player(filename);
		player->play();

		progress = new callback::repeat(1000, []() { logger::info("Current playback progress: " + String(player->seconds()) + "/" + String(player->duration()) + " seconds"); });
	}

	// net::connect(3);
}

void loop() {
	if (!fs::connected()) {
		logger::warn("USB device disconnected! Attempting to reconnect...");
		fs::connect();

		// request::netInit(); // Reinitialize network if USB is reconnected
		return;
	}

	// request::net(); // Try to keep the network connected

	// logger::info("Busy waiting... " + String(millis() / 1000) + "s");

	// if (!net::connected())
	// {
	//   logger::warn("Network disconnected! Attempting to reconnect...");
	//   net::connect(0);
	//   return;
	// }

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