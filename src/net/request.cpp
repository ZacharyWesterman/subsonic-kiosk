#include "request.hpp"
#include "../logger.hpp"
#include <algorithm>

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

		if ((request->status_code == MOVED_PERMANENTLY || request->status_code == TEMPORARY_REDIRECT || request->status_code == PERMANENT_REDIRECT) && id == "Location") {
			request->redirect_to = value;
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

Request::Request(const String &url) : requestUrl(url) {
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
	curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &Request::writeCallback);
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curlHandle, CURLOPT_HEADERFUNCTION, &Request::headerCallback);
	curl_easy_setopt(curlHandle, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(curlHandle, CURLOPT_TIMEOUT_MS, 10000L);

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

	if (bodyIndex >= responseBody.length() || downloaded_bytes >= content_length) {
		finished = true;
	}
	return data;
#else
	while (client.connected()) {
		int bytes = client.available();
		if (bytes == 0) {
			delay(10);
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

	if (!client.connected()) {
		finished = true;
	}

	if (downloaded_bytes >= content_length) {
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
	if (bodyIndex >= responseBody.length() || downloaded_bytes >= content_length) {
		finished = true;
	}
	return line;
#else
	while (client.connected()) {
		int bytes = client.available();
		bool lineComplete = false;

		if (bytes == 0) {
			delay(10);
			continue;
		}

		downloaded_bytes += bytes;

		line.reserve(line.length() + bytes);
		for (int i = 0; i < bytes; ++i) {
			char c = client.read();
			if (c == '\n') {
				lineComplete = true;
				break;
			}
			if (c != '\r') {
				line += c;
			}
		}

		if (lineComplete) {
			break;
		}
	}

	if (!client.connected()) {
		finished = true;
	}

	if (downloaded_bytes >= content_length) {
		finished = true;
	}

	return line;
#endif
}

String Request::text() {
#ifdef EMULATE
	return responseBody;
#else
	if (finished) {
		return responseBody;
	}

	while (client.connected()) {
		String line = readln();
		responseBody += line + "\n";
	}

	finished = true;
	return responseBody;
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
	return client.available();
#endif
}

std::vector<uint8_t> Request::data() {
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
#endif
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
