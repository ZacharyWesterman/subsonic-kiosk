#include "request.hpp"
#include "../logger.hpp"

namespace net {

Request::Request(WiFiClient &client) : client(client) {
	status = 400; // Default to bad request

	// Read the response status line
	String statusLine = readln();
	int i1 = statusLine.indexOf(' ');
	if (i1 != -1) {
		int i2 = statusLine.indexOf(' ', i1 + 1);
		if (i2 != -1) {
			status = statusLine.substring(i1 + 1, i2).toInt();
		}
	}

	if (!ok()) {
		finished = true;
	}
}

bool Request::done() const {
	return finished;
}

bool Request::ok() const {
	return status >= 200 && status < 300;
}

std::vector<uint8_t> Request::read(int chunkSize) {
	std::vector<uint8_t> data;
	if (finished) {
		return data; // Return empty vector if already finished
	}

	while (client.connected()) {
		int bytes = client.available();
		if (bytes == 0) {
			delay(10); // Wait for data to be available
			continue;
		}

		data.reserve(data.size() + bytes);
		for (int i = 0; i < bytes; ++i) {
			data.push_back(client.read());
			if (data.size() >= chunkSize) {
				break; // Stop reading if chunk size is reached
			}
		}

		if (data.size() >= chunkSize) {
			break; // Stop reading if chunk size is reached
		}
	}

	if (!client.connected()) {
		finished = true; // Mark as finished if connection is lost
	}

	return data;
}

String Request::readln() {
	String line;
	if (finished) {
		return line; // Return empty line if already finished
	}

	while (client.connected()) {
		int bytes = client.available();
		bool lineComplete = false;

		if (bytes == 0) {
			delay(10); // Wait for data to be available
			continue;
		}

		line.reserve(line.length() + bytes);
		for (int i = 0; i < bytes; ++i) {
			char c = client.read();
			if (c == '\n') {
				lineComplete = true; // End of line
				break;
			}
			if (c != '\r') // Ignore carriage return
			{
				line += c;
			}
		}

		if (lineComplete) {
			break;
		}
	}

	if (!client.connected()) {
		finished = true; // Mark as finished if connection is lost
	}

	return line;
}

String Request::text() {
	if (finished) {
		return responseBody;
	}

	bool foundBody = false;
	while (client.connected()) {
		if (readln().length() == 0) {
			foundBody = true; // Empty line indicates end of headers
			break;
		}
	}

	if (foundBody) {
		while (client.connected()) {
			String line = readln();
			responseBody += line + "\n"; // Append line to response body
		}
	}

	finished = true; // Mark as finished after reading the body
	return responseBody;
}

JsonDocument Request::json() {
	if (!ok()) {
		return JsonDocument(); // Return empty document if not OK
	}

	String body = text();
	JsonDocument doc;
	DeserializationError error = deserializeJson(doc, body.c_str());
	if (error) {
		logger::error("Failed to parse JSON: " + String(error.c_str()));
		return JsonDocument(); // Return empty document on error
	}
	return doc;
}

bool Request::ready() {
	return client.available();
}

std::vector<uint8_t> Request::data() {
	std::vector<uint8_t> availableData;

	if (finished) {
		return availableData;
	}

	if (!client.connected()) {
		finished = true;
		return availableData;
	}

	const int bytes = client.available();
	availableData.reserve(bytes);
	for (int i = 0; i < bytes; ++i) {
		availableData.push_back(client.read());
	}

	return availableData;
}

} // namespace net
