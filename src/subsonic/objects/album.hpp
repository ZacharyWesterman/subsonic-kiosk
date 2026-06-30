#pragma once

#include "../../polyfill/optional.hpp"
#include "song.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

/// @brief Represents a music album with metadata and related operations.
struct Album {
	/// @brief The list of songs the album contains, if any.
	optional<std::vector<Song>> songs;

	/// @brief The album ID.
	int id;

	/// @brief The album name.
	String name;

	/// @brief The album artist, or an empty string if none.
	String artist;

	/// @brief The ID of the album cover art, or an empty string if none.
	String coverArt;

	/// @brief The year the album was released, if any.
	optional<int> year;

	/// @brief The average rating of the album, if any.
	optional<int> averageRating;

	/// @brief The number of times the album has been played.
	int playCount;

	/// TODO created (datetime)
};

} // namespace subsonic
