/// @file require.hpp
#pragma once

namespace require {

/**
 * @brief Wait for the serial port to be ready.
 * @warning This function blocks until the serial port is ready.
 *         It is intended for debugging purposes and should not be used in production code.
 */
void serial();

} // namespace require

namespace request {
/**
 * @brief Check if the network is connected and attempt to reconnect if not.
 * @note This function should be called periodically to maintain network connectivity.
 *       It will try to reconnect if the network is not available, but will not block the main loop.
 * @warning This function does not handle network initialization; it assumes that the network has already
 *         been initialized with `netInit()`.
 */
void net();

/**
 * @brief Initialize the network connection using the configuration file.
 *
 * This function reads the network credentials from `network.json` and initializes the network.
 * It sets `NET_AVAILABLE` to true if the network is successfully initialized.
 *
 * @note The `network.json` file should be located in the root directory of the filesystem.
 *       The file should contain the SSID and password in JSON format.
 *       Example content:
 *       ```json
 *       {
 *         "ssid": "YourNetworkSSID",
 *         "password": "YourNetworkPassword"
 *       }
 *       ```
 */
void netInit();

} // namespace request
