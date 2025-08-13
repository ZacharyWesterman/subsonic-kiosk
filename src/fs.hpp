#pragma once

#include <Arduino.h>

namespace fs {

bool connect(int maxRetries = -1);

bool connected();

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

String _path(const String &path);

} // namespace fs

#include "fs/fileStream.hpp"
#include "fs/path.hpp"
