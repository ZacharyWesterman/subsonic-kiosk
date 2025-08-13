#pragma once

#include "fs.hpp"
#include "logger.hpp"
#include "net.hpp"

namespace require {

static bool FS_INITIAL_CONNECT = false;

/**
 * @brief Wait for the serial port to be ready.
 * @warning This function blocks until the serial port is ready.
 *         It is intended for debugging purposes and should not be used in production code.
 */
void serial() {
	Serial.begin(115200);
	while (!Serial) {
		pins::white();
		delay(50);
		pins::off();
		delay(1000);
	}
}
} // namespace require

namespace request {
/// @brief If true, network functionality is available for this device.
static bool NET_AVAILABLE = false;

/**
 * @brief Check if the network is connected and attempt to reconnect if not.
 * @note This function should be called periodically to maintain network connectivity.
 *       It will try to reconnect if the network is not available, but will not block the main loop.
 * @warning This function does not handle network initialization; it assumes that the network has already
 *         been initialized with `netInit()`.
 */
void net() {
	if (NET_AVAILABLE && !net::connected()) {
		net::tryConnect();
	}
}

/**
 * @brief Log an error message if the network configuration file cannot be read.
 */
static void netConfigError() {
	logger::error(
		"Failed to read network configuration file."
		"\nPlease create a file named 'network.json' with the following structure:\n"
		"Example content:\n"
		"{\n"
		"  \"ssid\": \"MyNetworkSSID\",\n"
		"  \"password\": \"MyNetworkPassword\"\n"
		"}");
}

/**
 * @brief Initialize the network connection using the configuration file.
 *
 * This function reads the network credentials from `network.json` and initializes the network.
 * It sets `NET_AVAILABLE` to true if the network is successfully initialized.
 *
 * @note The `network.json` file should be located in the root directory of the filesystem.
 *       The file should contain the SSID and password in JSON format.
 *       Example content:
 *       ```json
 *       {
 *         "ssid": "YourNetworkSSID",
 *         "password": "YourNetworkPassword"
 *       }
 *       ```
 */
void netInit() {
	NET_AVAILABLE = false;

	if (net::available()) {
		// Read network credentials from the filesystem
		fs::Path ssidPath("/network.json");
		if (!ssidPath.isFile()) {
			netConfigError();
			return;
		}

		// Sanity check in case the file is too large
		if (ssidPath.size() > 5 * 1024) {
			netConfigError();
			return;
		}

		// Deserialize the JSON file
		JsonDocument config;
		auto error = deserializeJson(config, ssidPath.read().c_str());
		if (error) {
			netConfigError();
			return;
		}

		// Make sure the JSON has the required fields
		if (!config.is<JsonObject>() || !config["ssid"].is<JsonString>() || !config["password"].is<JsonString>()) {
			netConfigError();
			return;
		}

		if (net::connected() && (String(config["ssid"].as<JsonString>().c_str()) == net::NETWORK_SSID && String(config["password"].as<JsonString>().c_str()) == net::NETWORK_PASS)) {
			return; // Already connected with the same credentials
		}

		if (net::connected()) {
			net::disconnect();
		}
		net::init(config["ssid"].as<JsonString>().c_str(), config["password"].as<JsonString>().c_str());
		NET_AVAILABLE = true;
	}
}

} // namespace request