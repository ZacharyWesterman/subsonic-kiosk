#include "net.hpp"
#include "logger.hpp"
#include "net/netClient.hpp"
#include "pins.hpp"
#include <Arduino.h>

#ifndef EMULATE
#include <WiFi.h>
#endif

#define REDIRECT_LIMIT 5

namespace net {

String NETWORK_SSID;
String NETWORK_PASS;
static unsigned long LAST_CONNECT_TIME = 0;
static bool CONNECTING = false;

bool available() {
#ifndef EMULATE
	return WiFi.status() != WL_NO_MODULE;
#else
	return true;
#endif
}

void init(const String &ssid, const String &password) {
	NETWORK_SSID = ssid;
	NETWORK_PASS = password;

	if (!available()) {
		logger::fatal("WiFi module not installed!");
	}
}

bool connected() {
#ifndef EMULATE
	return WiFi.status() == WL_CONNECTED;
#else
	return true;
#endif
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
#ifndef EMULATE
	WiFi.begin(NETWORK_SSID.c_str(), NETWORK_PASS.c_str());
#endif

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
#ifndef EMULATE
	WiFi.disconnect();
#endif
	logger::raw("Done.\n");
}

int ping(const char *host, int timeout) {
	if (!connected()) {
		return -1; // Not connected
	}

#ifndef EMULATE
	return WiFi.ping(host, timeout);
#else
	return 0;
#endif
}

NetClient client(const String &host, int port) {
	return NetClient(host, port);
}

Request get_request(const String &url, unsigned long timeout, int redirect) {
	// Split URL into host and path
	int protocolIndex = url.indexOf("://");
	int protocolEnd = (protocolIndex < 0) ? 0 : protocolIndex + 3; // Skip "://"
	int pathStart = url.indexOf("/", protocolEnd);
	int portStart = url.indexOf(":", protocolEnd);

	int hostEnd = (portStart >= 0 && portStart < pathStart) ? portStart : pathStart;

	String protocol = (protocolIndex < 0) ? "" : url.substring(0, protocolIndex);
	String host = url.substring(protocolEnd, hostEnd);
	String path = (pathStart < 0) ? "/" : url.substring(pathStart);

	int port = (protocol == "https") ? 443 : 80;
	if (portStart >= 0 && portStart < pathStart) {
		port = url.substring(portStart + 1, (pathStart >= 0 ? pathStart : url.length())).toInt();
	}

	logger::info("Creating GET request to [" + protocol + "] (" + host + ":" + String(port) + ") <" + path + ">");

	auto req = net::client(host, port).get(path, timeout);

	// Follow redirects up to the max redirect count.

	if (req.redirected() && redirect < REDIRECT_LIMIT) {
		req = get_request(req.location(), timeout, redirect + 1);
	}

	return req;
}

Request get(const String &url, unsigned long timeout) {
	return get_request(url, timeout, 0);
}

} // namespace net
