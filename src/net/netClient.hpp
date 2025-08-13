#pragma once

#include <ArduinoJson.h>
#include <WiFi.h>
#include <vector>

#include "../logger.hpp"

namespace net {

/**
 * @brief A class to represent an HTTP request.
 */
class Request {
	WiFiClient &client;
	bool finished = false;
	String responseBody;

public:
	/// The HTTP status code of the response.
	int status;

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
};

/**
 * @brief A class to represent a network client.
 * This class is used to create HTTP requests to a specified host and port.
 */
class NetClient {
	WiFiClient client;
	String host;
	int port;
	bool connected = false;

public:
	/**
	 * @brief Constructor for the NetClient class.
	 * @param host The hostname or IP address to connect to.
	 * @param port The port number to connect to.
	 */
	NetClient(const String &host, int port);

	/**
	 * @brief Destructor for the NetClient class.
	 * This will close the connection if it is still open.
	 */
	~NetClient();

	/**
	 * @brief Make a GET request to the specified path.
	 * @param path The path to request from the host.
	 * @return A Request object containing the response from the server.
	 */
	Request get(const String &path);
};

} // namespace net
