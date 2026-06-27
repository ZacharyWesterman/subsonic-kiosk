#pragma once

#include "../../polyfill/optional.hpp"
#include "song.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

/// @brief Represents a music album with metadata and related operations.
struct Album {
	optional<std::vector<Song>> songs;

	int id;
	String name;

	String artist;
	String coverArt;

	optional<int> year;
	optional<int> averageRating;

	int playCount;
	/// TODO created (datetime)
};

} // namespace subsonic
