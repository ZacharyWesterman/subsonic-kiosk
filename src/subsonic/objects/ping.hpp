/// @file ping.hpp
#pragma once

#include <Arduino.h>

namespace subsonic {

/// @brief Represents a Subsonic ping response with status, version, and type attributes.
struct Ping {
	/// @brief The status of the ping resopnse (e.g. "ok").
	String status;

	/// @brief The version returned with the ping response.
	String version;

	/// @brief The type returned with the ping response.
	String type;

	/**
	 * @brief Check if the ping was successful.
	 * @return True if the status is "ok", otherwise false.
	 */
	inline bool ok() const {
		return status == "ok";
	}
};

} // namespace subsonic
