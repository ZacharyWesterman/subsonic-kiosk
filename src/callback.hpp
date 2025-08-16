/// @file callback.hpp
#pragma once

#include <functional>

namespace callback {

/**
 * @brief A class to handle repeating callbacks at a specified interval.
 */
class repeat {
	unsigned long lastTime;
	unsigned long interval;
	std::function<void()> func;

public:
	/**
	 * @brief Constructor to initialize the repeat callback.
	 * @param interval The interval in milliseconds at which to call the function.
	 * @param func The function to call repeatedly.
	 */
	repeat(unsigned long interval, std::function<void()> func);

	/**
	 * @brief Check if the interval has passed and call the function if it has.
	 */
	void operator()();
};

} // namespace callback
