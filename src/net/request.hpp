#pragma once

#include "statusCodes.hpp"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <vector>

namespace net {

/**
 * @brief A class to represent an HTTP request.
 */
class Request {
	WiFiClient client;
	bool finished = false;
	String responseBody;
	StatusCode status_code;

public:
	/**
	 * @brief Constructor for the Request class.
	 * @param client The WiFiClient object to use for the request.
	 */
	Request(WiFiClient &client);

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

	/**
	 * @brief Read the response body in chunks.
	 * @param chunkSize The size of each chunk to read.
	 * @return A vector containing the bytes read from the response body.
	 *
	 * @warning This function will block until the specified number of bytes is
	 * available or the end of the response is reached.
	 */
	std::vector<uint8_t> read(int chunkSize = 1024);

	/**
	 * @brief Read a line from the response body.
	 * @return A String containing the line read from the response body.
	 *
	 * @warning This function will block until a line is available or the
	 * end of the response is reached.
	 */
	String readln();

	/**
	 * @brief Read the entire response body.
	 * @return A String containing the entire response body.
	 *
	 * @warning This function will block until the entire response body is read.
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
	 * @brief Get the data that's available to be read from the response.
	 * @return A vector containing the bytes that are available to be read.
	 */
	std::vector<uint8_t> data();
};

} // namespace net
