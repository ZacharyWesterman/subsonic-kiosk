#pragma once

#include "../../polyfill/optional.hpp"
#include <Arduino.h>

namespace subsonic {

/// @brief Represents a Subsonic song object with metadata and streaming capabilities.
struct Song {
	/// @brief A unique identifier for the song.
	int id;

	/// @brief The identifier of the parent directory or collection.
	int parent;

	/// @brief The title of the song.
	String title;

	/// @brief The name of the album that the song belongs to.
	String album;

	/// TODO created (datetime)

	String contentType;
	String suffix;
	String path;
	String type;

	/// @brief The number of times the song has been played.
	int playCount;

	unsigned long size;
	int duration;
	// int bitRate; (OPTIONAL)
	int albumId;

	// optional<String> coverArt;
	// optional<String> artist;
	// optional<String> artistId;
	// optional<int> track;
	// optional<int> year;
	// optional<String> genre;
	// optional<int> discNumber;
	// optional<String> transcodedContentType;
	// optional<String> transcodedSuffix;
	// optional<float> averageRating;

	// String uri() const;
};

} // namespace subsonic
