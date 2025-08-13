#pragma once

#include <Arduino.h>

namespace logger {

/**
 * @brief Produce a fatal error message and halt the program.
 * @param message The error message to log.
 */
void fatal(const String &message);

/**
 * @brief Produce an error message.
 * @param message The error message to log.
 */
void error(const String &message);

/**
 * @brief Produce a warning message.
 * @param message The warning message to log.
 */
void warn(const String &message);

/**
 * @brief Produce an informational message.
 * @param message The informational message to log.
 * @param lineBreak Whether to add a line break after the message.
 */
void info(const String &message, bool lineBreak = true);

/**
 * @brief Print text to the console without any formatting.
 * @param message The message to print.
 */
void raw(const String &message);

#ifdef DEBUG
/**
 * @brief Test the logger functionality.
 * This function is used to verify that the logger is working correctly.
 * It will produce a series of log messages at different levels.
 * @note This is used for debugging purposes and will not be included in production builds.
 */
void test();
#endif

} // namespace logger
