#pragma once

#include <Arduino.h>

namespace fs {

/**
 * @brief Waits for the USB device to be connected and mounts the filesystem.
 * @param maxRetries The maximum number of retries to wait for the USB device to be connected.
 *                   If set to -1, it will wait indefinitely until the device is connected.
 * @return True if initialization was successful, false otherwise.
 */
bool connect(int maxRetries = -1);

/**
 * @brief Checks if the USB device is connected and the filesystem is mounted.
 * @return True if the USB device is connected and the filesystem is mounted, false otherwise.
 */
bool connected();

/**
 * @brief Unmounts the filesystem.
 */
void disconnect();

/**
 * @brief Returns the total space on the USB device in bytes.
 * @return The amount of total space in bytes.
 */
size_t size();

/**
 * @brief Returns the used space on the USB device in bytes.
 * @return The amount of used space in bytes.
 */
size_t used();

/**
 * @brief Returns the free space on the USB device in bytes.
 * @return The amount of free space in bytes.
 */
size_t free();

/**
 * @brief Returns a formatted path for the given file path.
 * @param path The original file path.
 * @return The formatted file path.
 */
String _path(const String &path);

} // namespace fs

#include "fs/fileStream.hpp"
#include "fs/path.hpp"
