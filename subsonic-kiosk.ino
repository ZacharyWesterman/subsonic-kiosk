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

	auto res = client->folders().await();
	for (auto folder : res.value()) {
		Serial.println(String("{ id: ") + folder.id + ", name: " + folder.name + " }");
	}
}

void loop() {
#ifdef EMULATE
	exit(0);
#endif
}
