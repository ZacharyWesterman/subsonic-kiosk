#include "net.hpp"
#include "logger.hpp"
#include "net/netClient.hpp"
#include "pins.hpp"
#include <Arduino.h>
#include <WiFi.h>

namespace net {

String NETWORK_SSID;
String NETWORK_PASS;
static unsigned long LAST_CONNECT_TIME = 0;
static bool CONNECTING = false;

bool available() {
	return WiFi.status() != WL_NO_MODULE;
}

void init(const String &ssid, const String &password) {
	NETWORK_SSID = ssid;
	NETWORK_PASS = password;

	if (!available()) {
		logger::fatal("WiFi module not installed!");
	}
}

bool connected() {
	return WiFi.status() == WL_CONNECTED;
}

void tryConnect() {
	if (!available()) {
		logger::fatal("WiFi module not installed!");
	}

	if (connected()) {
		logger::info("Connected to WiFi network: " + NETWORK_SSID);
		pins::off();
		CONNECTING = false;
		return;
	}

	if (CONNECTING) {
		unsigned long now = millis();
		if (abs(now - LAST_CONNECT_TIME) < 5000) {
			return; // Avoid spamming connection attempts
		}
	}

	pins::blue();
	logger::info("Connecting to WiFi network: " + NETWORK_SSID);
	WiFi.begin(NETWORK_SSID.c_str(), NETWORK_PASS.c_str());

	CONNECTING = true;
	LAST_CONNECT_TIME = millis();

	if (connected()) {
		logger::info("Connected to WiFi network: " + NETWORK_SSID);
		pins::off();
		CONNECTING = false;
		return;
	}
}

void disconnect() {
	logger::info("Disconnecting from WiFi... ", false);
	WiFi.disconnect();
	logger::raw("Done.\n");
}

int ping(const char *host, int timeout) {
	if (!connected()) {
		return -1; // Not connected
	}

	return WiFi.ping(host, timeout);
}

NetClient client(const String &host, int port) {
	return NetClient(host, port);
}

} // namespace net
