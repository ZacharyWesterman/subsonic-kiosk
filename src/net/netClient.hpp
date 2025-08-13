#pragma once

#include <ArduinoJson.h>
#include <WiFi.h>
#include <vector>

#include "../logger.hpp"

namespace net {

class Request {
	WiFiClient &client;
	bool finished = false;
	String responseBody;

public:
	int status;

	Request(WiFiClient &client);

	inline operator bool() const {
		return ok();
	}

	bool done() const;

	bool ok() const;

	std::vector<uint8_t> read(int chunkSize = 1024);

	String readln();

	String text();

	JsonDocument json();
};

class NetClient {
	WiFiClient client;
	String host;
	int port;
	bool connected = false;

public:
	NetClient(const String &host, int port);

	~NetClient();

	Request get(const String &path);
};

} // namespace net
