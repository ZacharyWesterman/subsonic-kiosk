#pragma once

#include <Arduino.h>

namespace subsonic {

/// @brief Represents a music artist.
struct Artist {
	/// @brief A unique identifier for the artist.
	String id;

	/// @brief The name of the artist.
	String name;
};

} // namespace subsonic
