/// @file pins.hpp
#pragma once

namespace pins {
/**
 * @brief Set the built-in RGB LED to red.
 */
void red();

/**
 * @brief Set the built-in RGB LED to green.
 */
void green();

/**
 * @brief Set the built-in RGB LED to blue.
 */
void blue();

/**
 * @brief Set the built-in RGB LED to cyan.
 */
void cyan();

/**
 * @brief Set the built-in RGB LED to magenta.
 */
void magenta();

/**
 * @brief Set the built-in RGB LED to yellow.
 */
void yellow();

/**
 * @brief Set the built-in RGB LED to white.
 */
void white();

/**
 * @brief Turn off the built-in RGB LED.
 */
void off();

/**
 * @brief Turn off the built-in RGB LED.
 */
inline void black();

/**
 * @brief Initialize the pins for the built-in RGB LED.
 */
void init();

} // namespace pins
