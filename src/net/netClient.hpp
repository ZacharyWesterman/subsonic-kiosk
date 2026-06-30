/// @file netClient.hpp
#pragma once

#include "request.hpp"

#ifndef EMULATE
#include <WiFi.h>
#endif

namespace net {

/**
 * @brief A class to represent a network client.
 * This class is used to create HTTP requests to a specified host and port.
 */
class NetClient {
#ifndef EMULATE
	WiFiClient client;
#endif
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
	 * @param timeout The timeout for the query in milliseconds.
	 * @return A Request object containing the response from the server.
	 */
	Request get(const String &path, unsigned long timeout = 10000);
};

} // namespace net
