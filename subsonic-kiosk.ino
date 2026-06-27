// #include "src/audio.hpp"
// #include "src/callback.hpp"
#include "src/emulation_helpers.hpp"
#include "src/fs.hpp"
#include "src/net.hpp"
#include "src/pins.hpp"
#include "src/require.hpp"
#include "src/subsonic/client.hpp"

subsonic::Client *client;

void setup() {
	pins::init();

	fs::connect();			// Ensure filesystem is connected
	request::netInit(); // Initialize network connection

	while (!net::connected()) {
		request::net();
	}

	require::serial(); // Wait for serial port to open.

	// Load subsonic client from file
	//  Read network credentials from the filesystem
	fs::Path configPath("/subsonic.json");
	if (!configPath.isFile()) {
		return;
	}

	// Sanity check in case the file is too large
	if (configPath.size() > 5 * 1024) {
		return;
	}

	// Deserialize the JSON file
	JsonDocument config;
	auto error = deserializeJson(config, configPath.read().c_str());
	if (error) {
		return;
	}

	// Make sure the JSON has the required fields
	if (!json_is_obj(config) || !json_is_str(config["host"]) || !json_is_str(config["username"]) || !json_is_str(config["md5sum"]) || !json_is_str(config["salt"])) {
		return;
	}

	client = new subsonic::Client(json_to(String, config["host"]), json_to(String, config["username"]), json_to(String, config["md5sum"]), json_to(String, config["salt"]));

	// DEBUG: LOAD STUFF HERE
	auto res = client->search("machine girl").await();

	if (!res.has_value()) {
		Serial.println("SEARCH FAILED!");
		return;
	}

	auto &search = res.value();

	Serial.println("Search Results:");
	int i = 0;

	Serial.println("\n<<ARTISTS>>");
	i = 0;
	for (const auto &artist : search.artists) {
		Serial.println("  " + String(++i) + ". " + artist.name);
	}

	Serial.println("\n<<ALBUMS>>");
	i = 0;
	for (const auto &album : search.albums) {
		Serial.println("  " + String(++i) + ". " + album.name + " [by " + album.artist + "]");
	}

	Serial.println("\n<<SONGS>>");
	i = 0;
	for (const auto &song : search.songs) {
		Serial.println("  " + String(++i) + ". " + song.title + " [by " + song.artist + "]");
	}
}

void loop() {
#ifdef EMULATE
	exit(0);
#endif
}
