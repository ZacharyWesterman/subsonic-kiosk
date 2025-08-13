#pragma once

#include "net/netClient.hpp"

namespace net {

/// @brief The SSID of the network to connect to.
extern String NETWORK_SSID;

/// @brief The password for the network to connect to.
extern String NETWORK_PASS;

/**
 * @brief Check if the network is available.
 * @return True if the network is available, false otherwise.
 */
bool available();

/**
 * @brief Initialize the network connection.
 * @param ssid The SSID of the network to connect to.
 * @param password The password for the network.
 */
void init(const String &ssid, const String &password);

/**
 * @brief Check if the network is connected.
 * @return True if the network is connected, false otherwise.
 */
bool connected();

/**
 * @brief Try to connect to the network without blocking.
 * This function will attempt to connect to the network and return immediately.
 * If the connection is successful, connected() will eventually return true.
 *
 * @note This function can be safely called in a hard loop to keep trying to connect,
 * but due to quirks in the WiFi library, it may occasionally block for a fraction of a second.
 */
void tryConnect();

/**
 * @brief Disconnect from the network.
 * This function will disconnect from the current network.
 */
void disconnect();

/**
 * @brief Ping a host to check if it is reachable.
 * @param host The hostname or IP address to ping.
 * @param timeout The timeout in milliseconds for the ping operation.
 * @return -1 if the network is not connected, otherwise the ping response time in milliseconds.
 */
int ping(const char *host, int timeout = 5000);

/**
 * @brief Create a network client for the specified host and port.
 * @param host The hostname or IP address to connect to.
 * @param port The port number to connect to.
 * @return A NetClient object for the specified host and port.
 */
NetClient client(const String &host, int port = 80);

} // namespace net
