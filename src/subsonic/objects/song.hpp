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

	/// @brief The artist name, or an empty string if none.
	String artist;

	/// TODO created (datetime)

	/// @brief The mime type of the song content, e.g. `audio/mpeg`.
	String contentType;

	/// @brief The file suffix, e.g. `mp3`.
	String suffix;

	/// @brief The actual filename of the object as stored on the server, e.g. `song.mp3`.
	String path;

	/// @brief The number of times the song has been played.
	int playCount;

	/// @brief The size of the file in bytes.
	unsigned long size;

	/// @brief The duration of the songs in seconds.
	int duration;

	// int bitRate; (OPTIONAL)

	/// @brief The ID of the album this song belongs to.
	int albumId;

	// optional<String> coverArt;
	// optional<String> artistId;

	/// @brief The track number of this song, if any.
	optional<int> track;

	/// @brief The year this song was released, if any.
	optional<int> year;

	// optional<String> genre;

	/// @brief The disk this song was on, if the album comes from multiple disks.
	optional<int> diskNumber;

	// optional<String> transcodedContentType;
	// optional<String> transcodedSuffix;

	/// @brief The average rating of this song, if any.
	optional<int> averageRating;

	// String uri() const;
};

} // namespace subsonic
