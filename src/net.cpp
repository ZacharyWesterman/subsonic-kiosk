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

Request get(const String &url) {
	// Split URL into host and path
	int protocolIndex = url.indexOf("://");
	int protocolEnd = (protocolIndex < 0) ? 0 : protocolIndex + 3; // Skip "://"
	int pathStart = url.indexOf("/", protocolEnd);

	String protocol = (protocolIndex < 0) ? "" : url.substring(0, protocolIndex);
	String host = url.substring(protocolEnd, pathStart);
	String path = (pathStart < 0) ? "/" : url.substring(pathStart);

	logger::info("Creating GET request to [" + protocol + "] (" + host + ") <" + path + ">");

	int port = (protocol == "https") ? 443 : 80;

	return Request(net::client(host, port).get(path));
}

} // namespace net
