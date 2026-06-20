#include "request.hpp"
#include "../logger.hpp"
#include <algorithm>

#define IN_LOOP_WAIT_UNTIL_TIMEOUT_ELSE(bytes, on_timeout) \
	if (!bytes) { \
		waitWithTimeout(); \
		if (finished) { \
			on_timeout; \
		} \
		continue; \
	} else { \
		waitStart = millis(); \
	}

namespace net {

#ifdef EMULATE
size_t Request::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	const size_t total = size * nmemb;
	if (total == 0 || !userdata) {
		return 0;
	}

	Request *request = static_cast<Request *>(userdata);
	request->responseBody.concat(ptr, total);
	request->downloaded_bytes = request->responseBody.length();
	return total;
}

size_t Request::headerCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	const size_t total = size * nmemb;
	if (total == 0 || !userdata) {
		return 0;
	}

	Request *request = static_cast<Request *>(userdata);
	String header;
	header.concat(ptr, total);
	header.trim();

	if (header.length() == 0) {
		return total;
	}

	const int colon = header.indexOf(':');
	if (colon != -1) {
		String id = header.substring(0, colon);
		String value = header.substring(colon + 1);
		value.trim();

		if (id == "Content-Length") {
			request->content_length = value.toInt();
		}
	} else if (header.startsWith("HTTP/")) {
		const int firstSpace = header.indexOf(' ');
		const int secondSpace = header.indexOf(' ', firstSpace + 1);
		if (firstSpace != -1 && secondSpace != -1) {
			const String code = header.substring(firstSpace + 1, secondSpace);
			request->status_code = static_cast<StatusCode>(code.toInt());
		}
	}

	return total;
}

Request::Request(const String &url, unsigned long timeout) : requestUrl(url) {
	requestTimeoutAt = millis() + timeout;

	status_code = BAD_REQUEST;
	content_length = 0;
	downloaded_bytes = 0;
	finished = false;

	curlHandle = curl_easy_init();
	if (!curlHandle) {
		logger::error("Failed to initialize curl.");
		finished = true;
		return;
	}

	curl_easy_setopt(curlHandle, CURLOPT_URL, requestUrl.c_str());
	curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 0L);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &Request::writeCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, &Request::headerCallback);
	curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT_MS, timeout);

	const CURLcode code = curl_easy_perform(curlHandle);
	if (code != CURLE_OK) {
		logger::error("Failed to perform request to " + requestUrl + ": " + String(curl_easy_strerror(code)));
		status_code = BAD_REQUEST;
		finished = true;
	} else {
		long responseCode = 0;
		curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &responseCode);
		if (responseCode > 0) {
			status_code = static_cast<StatusCode>(responseCode);
		}

		if (content_length == 0 && downloaded_bytes > 0) {
			content_length = downloaded_bytes;
		}

		logger::info("curl response for " + requestUrl + " => status=" + String(static_cast<int>(status_code)) + ", bytes=" + String(responseBody.length()) + ", content-length=" + String(content_length));
		finished = (responseBody.length() == 0);
	}

	curl_easy_cleanup(curlHandle);
	curlHandle = nullptr;
	logger::info("curl complete for " + requestUrl + ": bodyBytes=" + String(responseBody.length()) + ", contentLength=" + String(content_length) + ", status=" + String(static_cast<int>(status_code)) + ", finished=" + String(finished ? 1 : 0));
}
#else
Request::Request(WiFiClient &client, unsigned long timeout) : client(client), responseBody(), content_start(0), content_length(-1), downloaded_bytes(0), timeout(timeout), status_code(BAD_REQUEST), finished(false), found_content(false) {
	waitStart = millis();

	// Read the status code (first line of response)
	String line = readln();
	int i1 = line.indexOf(' ');
	if (i1 == -1) {
		finished = true;
		return;
	}
	int i2 = line.indexOf(' ', i1 + 1);
	if (i2 == -1) {
		i2 = line.length();
	}
	status_code = static_cast<StatusCode>(line.substring(i1 + 1, i2).toInt());

	// Read headers
	String id, value;
	while (!finished) {
		line = readln();
		if (line.length() == 0) {
			// Empty line indicates end of headers
			found_content = true;
			break;
		}

		const int i1 = line.indexOf(": ");
		if (i1 == -1) {
			continue;
		}
		id = line.substring(0, i1);
		value = line.substring(i1 + 2);

		if (id == "Content-Length") {
			content_length = value.toInt();
		}
	}

	// Read any more data that was received after the headers.
	if (content_length != (uint64_t)-1) {
		responseBody.reserve(content_start + content_length);
	}
	while (true) {
		while (client.available()) {
			char c = client.read();
			downloaded_bytes++;
			responseBody.concat(c);
		}

		delay(1);
		if (!client.available()) {
			break;
		}
	}

	if (found_content && downloaded_bytes - content_start >= content_length) {
		finished = true;
	}
}
#endif

bool Request::done() const {
	return finished;
}

bool Request::ok() const {
	return status_code >= 200 && status_code < 300;
}

StatusCode Request::status() const {
	return status_code;
}

void Request::process() {
#ifdef EMULATE
	read(1024);
#else
	if (!client.connected()) {
		return;
	}

	int bytes = client.available();
	if (bytes == 0) {
		// if (millis() > requestTimeoutAt) {
		// 	finished = true;
		// 	status_code = GATEWAY_TIMEOUT;
		// }

		return;
	}

	read(bytes);
#endif
}

std::vector<uint8_t> Request::read(int chunkSize) {
	std::vector<uint8_t> data;
	if (finished) {
		return data;
	}

#ifdef EMULATE
	if (bodyIndex >= responseBody.length()) {
		finished = true;
		return data;
	}

	const size_t bytes = std::min<size_t>(static_cast<size_t>(chunkSize), responseBody.length() - bodyIndex);
	for (size_t i = 0; i < bytes; ++i) {
		data.push_back(static_cast<uint8_t>(responseBody[bodyIndex + i]));
	}
	bodyIndex += bytes;
	downloaded_bytes = bodyIndex;

	if (content_length > 0 && downloaded_bytes >= content_length) {
		finished = true;
	}
	return data;
#else
	while (client.connected() || client.available() > 0) {
		int bytes = client.available();
		if (bytes == 0) {
			waitWithTimeout();
			if (finished) {
				break;
			}
			continue;
		}

		downloaded_bytes += bytes;

		data.reserve(data.size() + bytes);
		for (int i = 0; i < bytes; ++i) {
			data.push_back(client.read());
			if (data.size() >= chunkSize) {
				break;
			}
		}

		if (data.size() >= chunkSize) {
			break;
		}
	}

	if ((!client.connected() && client.available() == 0) || (content_length > 0 && downloaded_bytes >= content_length)) {
		finished = true;
	}

	return data;
#endif
}

String Request::readln() {
	String line;
	if (finished) {
		return line;
	}

#ifdef EMULATE
	if (bodyIndex >= responseBody.length()) {
		finished = true;
		return line;
	}

	while (bodyIndex < responseBody.length()) {
		char c = responseBody.charAt(bodyIndex++);
		if (c == '\n') {
			break;
		}
		if (c != '\r') {
			line += c;
		}
	}
	bodyIndex = std::min<size_t>(bodyIndex, responseBody.length());
	downloaded_bytes = bodyIndex;
	if (bodyIndex >= responseBody.length() || (content_length > 0 && downloaded_bytes >= content_length)) {
		finished = true;
	}
	return line;
#else

	while (!finished) {
		unsigned int bytes = client.available();
		IN_LOOP_WAIT_UNTIL_TIMEOUT_ELSE(bytes, return line);

		line.reserve(bytes + line.length());
		responseBody.reserve(bytes + responseBody.length());
		bool endOfLine = false;
		for (unsigned int i = 0; i < bytes; i++) {
			char c = client.read();
			downloaded_bytes++;
			responseBody.concat(c);
			if (!found_content) {
				content_start++;
			}
			if (c == '\r') {
				continue;
			}
			if (c == '\n') {
				endOfLine = true;
				break;
			}
			line.concat(c);
		}

		if (found_content && downloaded_bytes - content_start >= content_length) {
			finished = true;
		}

		if (endOfLine) {
			break;
		}
	}

	return line;
#endif
}

void Request::collect() {
#ifndef EMULATE
	if (content_length != (uint64_t)-1) {
		responseBody.reserve(content_start + content_length);
	}

	while (!finished) {
		unsigned int bytes = client.available();
		IN_LOOP_WAIT_UNTIL_TIMEOUT_ELSE(bytes, break);

		// read as much data as possible.
		while (true) {
			while (client.available()) {
				char c = client.read();
				downloaded_bytes++;
				responseBody.concat(c);
			}

			delay(1);
			if (!client.available()) {
				break;
			}
		}

		if (found_content && downloaded_bytes - content_start >= content_length) {
			finished = true;
		}
	}
#endif
}

String Request::text() {
#ifdef EMULATE
	return responseBody;
#else
	collect();

	if (!found_content) {
		return "";
	}

	return responseBody.substring(content_start, responseBody.length());
#endif
}

JsonDocument Request::json() {
	if (!ok()) {
		return JsonDocument();
	}

	String body = text();
	JsonDocument doc;
	DeserializationError error = deserializeJson(doc, body.c_str());
	if (error) {
		logger::error("Failed to parse JSON: " + String(error.c_str()));
		return JsonDocument();
	}
	return doc;
}

bool Request::ready() {
#ifdef EMULATE
	return bodyIndex < responseBody.length();
#else
	return client.available() > 0 || (!client.connected() && !finished);
#endif
}

std::vector<uint8_t> Request::stream() {
	std::vector<uint8_t> availableData;

#ifdef EMULATE
	const size_t bytes = responseBody.length() - bodyIndex;
	if (bytes == 0) {
		finished = true;
		return availableData;
	}

	availableData.reserve(bytes);
	for (size_t i = 0; i < bytes; ++i) {
		availableData.push_back(static_cast<uint8_t>(responseBody[bodyIndex + i]));
	}
	bodyIndex = responseBody.length();
	downloaded_bytes = bodyIndex;
	finished = true;
	logger::info("request.data() copied " + String(bytes) + " bytes from " + requestUrl);
	return availableData;
#else
	if (finished) {
		return availableData;
	}

	// Load any cached data first
	if (found_content && content_start < downloaded_bytes) {
		availableData.reserve(content_start - downloaded_bytes);

		for (size_t i = content_start; i < downloaded_bytes; i++) {
			availableData.push_back((uint8_t)responseBody[i]);
		}
		// Adjust index so we don't load again on subsequent calls.
		content_start = downloaded_bytes;
	}

	bool downloaded = false;
	while (!finished && !downloaded) {
		unsigned int bytes = client.available();
		IN_LOOP_WAIT_UNTIL_TIMEOUT_ELSE(bytes, return availableData);

		// read as much data as possible.
		while (true) {
			while (client.available()) {
				uint8_t c = client.read();
				downloaded_bytes++;
				downloaded = true;
				availableData.push_back(c);
			}

			delay(1);
			if (!client.available()) {
				break;
			}
		}

		if (found_content && downloaded_bytes - content_start >= content_length) {
			finished = true;
		}
	}

	return availableData;
#endif
}

uint64_t Request::length() const {
	return content_length;
}

uint64_t Request::downloaded() const {
	return downloaded_bytes;
}

float Request::progress() const {
	if (content_length == 0 || !found_content) {
		return 0.0f;
	}
	return static_cast<float>(downloaded_bytes - content_start) / static_cast<float>(content_length);
}

bool Request::redirected() const {
	return status_code == MOVED_PERMANENTLY || status_code == TEMPORARY_REDIRECT || status_code == PERMANENT_REDIRECT;
}

const String Request::location() const {
	int begin = findHeader("Location");

	if (begin < 0) {
		return "";
	}

	int end = responseBody.indexOf('\n', begin + 1);
	return responseBody.substring(begin, end < 0 ? responseBody.length() : end);
}

void Request::waitWithTimeout() {
	if (millis() - waitStart > timeout) {
		finished = true;
		status_code = GATEWAY_TIMEOUT;
		return;
	}

	delay(10);
}

int Request::findHeader(const char *name) {
	int index = 0;
	while ((index = responseBody.indexOf('\n', index)) != -1) {
		// 1. No more data, so header wasn't found.
		// 2. Two end lines indicates the end of headers.
		if (index == responseBody.length() - 1 || responseBody[index + 1] == '\n') {
			return -1;
		}

		int n = 0;
		int begin = ++index;
		int end = responseBody.length();
		bool found = false;
		for (int i = begin; i < end; i++) {
			char find_c = name[n++];
			char search_c = responseBody[i];

			// We found an exact header match!
			// Return the index *after* the header name.
			if (!find_c && search_c == ' ') {
				return i + 1;
			}

			// This isn't the header we're looking for,
			// move to the next header.
			if (search_c != find_c) {
				break;
			}
		}
	}

	return -1;
}

} // namespace net
