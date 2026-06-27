#pragma once

#include <Arduino.h>

namespace subsonic {

/// @brief Represents a Subsonic folder.
struct Folder {
	/// @brief The name of the folder.
	String name;

	/// @brief The unique ID of the folder.
	int id;
};

} // namespace subsonic
