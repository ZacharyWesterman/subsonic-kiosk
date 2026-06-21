/// @file request.hpp
#pragma once

#include "statusCodes.hpp"
#include <ArduinoJson.h>
#include <vector>

#ifdef EMULATE
#include <Arduino.h>
#include <curl/curl.h>
#else
#include <WiFi.h>
#endif

namespace net {

/**
 * @brief A class to represent an HTTP request.
 */
class Request {
#ifdef EMULATE
	CURL *curlHandle = nullptr;
	String requestUrl;
	size_t bodyIndex = 0;
	static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t headerCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
#else
	WiFiClient client;
#endif
	String responseBody;
	uint64_t content_start;
	uint64_t content_length;
	uint64_t downloaded_bytes;
	unsigned long timeout;
	unsigned long waitStart;
	int chunkSize;
	StatusCode status_code;
	bool finished;
	bool found_content;
	bool isChunked;

	void waitWithTimeout();
	int findHeader(const char *name) const;

	/**
	 * @brief Read a line from the response body.
	 * @return A String containing the line read from the response body.
	 *
	 * @warning This function will block until a line is available or the
	 * end of the response is reached.
	 */
	String readln();

	void collect();

	int readChunkSize();

public:
	/**
	 * @brief Constructor for the Request class.
	 * @param client The WiFiClient object to use for the request.
	 */
#ifdef EMULATE
	Request(const String &url, unsigned long timeout);
#else
	Request(WiFiClient &client, unsigned long timeout);
#endif

	/**
	 * @brief Boolean conversion operator to check if the request is successful.
	 * @return True if the request is successful, false otherwise.
	 */
	inline operator bool() const {
		return ok();
	}

	/**
	 * @brief Check if the request is finished.
	 * @return True if the request is finished, false otherwise.
	 */
	bool done() const;

	/**
	 * @brief Check if the request was successful.
	 * @return True if the request was successful, false otherwise.
	 */
	bool ok() const;

	/**
	 * @brief Get the HTTP status code of the response.
	 * @return The HTTP status code of the response.
	 */
	StatusCode status() const;

	void process();

	/**
	 * @brief Read the entire response body.
	 * @return A String containing the entire response body.
	 *
	 * @warning This function will block until the entire response body is read.
	 * Avoid using this method if you expect the response to be very large
	 * (e.g. downloading files).
	 *
	 * @see stream()
	 */
	String text();

	/**
	 * @brief Read the response body as a JSON document.
	 * @return A JsonDocument containing the parsed JSON response.
	 *
	 * @warning This function will block until the entire response body is read
	 * and parsed as JSON. If the response is not valid JSON, it will return an
	 * empty JsonDocument.
	 */
	JsonDocument json();

	/**
	 * @brief Check if data is ready to be read from the response.
	 * @return True if data is ready, false otherwise.
	 */
	bool ready();

	/**
	 * @brief Stream response data from the request without storing all of it.
	 *
	 * This first reads any cached data, then streams data from the network.
	 *
	 * @note This will block while waiting to receive more data from the network.
	 *
	 * @warning Avoid using stream() and text()/json() on the same request object;
	 * stream() will actively consume data such that it will no longer be available
	 * to other methods.
	 *
	 * @return A vector containing the bytes read from the network.
	 * Returns an empty vector if there's no more data to be read.
	 */
	std::vector<uint8_t> stream();

	/**
	 * @brief Get the content length of the response.
	 * @return The content length of the response in bytes.
	 */
	uint64_t length() const;

	/**
	 * @brief Get the number of bytes downloaded so far.
	 * @return The number of bytes downloaded so far.
	 */
	uint64_t downloaded() const;

	/**
	 * @brief Get the ratio of bytes downloaded to content length.
	 * @return The ratio of bytes downloaded to content length.
	 */
	float progress() const;

	/**
	 * @brief Check whether the response was a redirect.
	 * @return True if redirected, false otherwise.
	 */
	bool redirected() const;

	/**
	 * @brief Get the redirect url, if any.
	 * @return The url to be redirected to, or an empty string if not redirecting.
	 */
	String location() const;
};

} // namespace net
