#include "request.hpp"
#include "../logger.hpp"

namespace net {

Request::Request(WiFiClient &client) : client(client) {
	status_code = BAD_REQUEST; // Default to bad request
	content_length = 0;
	downloaded_bytes = 0;

	// Read the response status line
	String line = readln();
	int i1 = line.indexOf(' ');
	if (i1 != -1) {
		int i2 = line.indexOf(' ', i1 + 1);
		if (i2 != -1) {
			status_code = static_cast<StatusCode>(line.substring(i1 + 1, i2).toInt());
		}
	}

	// Read headers
	while (client.connected()) {
		line = readln();
		if (line.length() == 0) {
			// Empty line indicates end of headers
			break;
		}
		String id, value;
		const int i1 = line.indexOf(": ");
		if (i1 != -1) {
			id = line.substring(0, i1);
			value = line.substring(i1 + 2);
		}

		if (id == "Content-Length") {
			content_length = value.toInt();
		}

		if ((status_code == MOVED_PERMANENTLY || status_code == TEMPORARY_REDIRECT || status_code == PERMANENT_REDIRECT) && id == "Location") {
			redirect_to = value;
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
	return status_code >= 200 && status_code < 300;
}

StatusCode Request::status() const {
	return status_code;
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

		downloaded_bytes += bytes;

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

	if (downloaded_bytes >= content_length) {
		finished = true;
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

		downloaded_bytes += bytes;

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

	if (downloaded_bytes >= content_length) {
		finished = true;
	}

	return line;
}

String Request::text() {
	if (finished) {
		return responseBody;
	}

	while (client.connected()) {
		String line = readln();
		responseBody += line + "\n"; // Append line to response body
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

	logger::info(String(client.connected()));

	if (!client.connected()) {
		finished = true;
		return availableData;
	}

	const int bytes = client.available();
	availableData.reserve(bytes);
	for (int i = 0; i < bytes; ++i) {
		availableData.push_back(client.read());
	}

	downloaded_bytes += bytes;
	if (downloaded_bytes >= content_length) {
		finished = true;
	}

	return availableData;
}

uint64_t Request::length() const {
	return content_length;
}

uint64_t Request::downloaded() const {
	return downloaded_bytes;
}

float Request::progress() const {
	if (content_length == 0) {
		return 0.0f;
	}
	return static_cast<float>(downloaded_bytes) / static_cast<float>(content_length);
}

bool Request::redirected() const {
	return status_code == MOVED_PERMANENTLY || status_code == TEMPORARY_REDIRECT || status_code == PERMANENT_REDIRECT;
}

const String &Request::location() const {
	return redirect_to;
}

} // namespace net
