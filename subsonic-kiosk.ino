#include "src/audio.hpp"
#include "src/callback.hpp"
#include "src/fs.hpp"
#include "src/net.hpp"
#include "src/pins.hpp"
#include "src/require.hpp"
#include "src/util/downloadQueue.hpp"

audio::Player *player;

callback::repeat *progress = nullptr;

util::DownloadQueue queue;
int id = 0;

void setup() {
	pins::init();

#ifdef DEBUG
	require::serial(); // Initialize serial communication
#endif

	fs::connect();			// Ensure filesystem is connected
	request::netInit(); // Initialize network connection

	while (!net::connected()) {
		request::net();
	}

	// Initialize the audio player with a specific file, if supported.
	/*
	fs::Path filename("/spark.wav");
	if (filename.isFile() && audio::supported(filename.ext())) {
		player = new audio::Player(filename);
		player->play();

		progress = new callback::repeat(1000, []() { logger::info("Current playback progress: " + String(player->seconds()) + "/" + String(player->duration()) + " seconds"); });
	}
	*/
	id = queue.download(fs::Path("/test.html"), "google.com", "/");
}

void loop() {
	queue.process();
	if (queue.finished(id)) {
		logger::info("Download finished for ID: " + String(id));
		queue.cleanup();
	}

	/*
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
	*/
}
