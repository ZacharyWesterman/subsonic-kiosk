#pragma once

#include "../../polyfill/optional.hpp"
#include "subsonic_object.hpp"
#include <Arduino.h>

namespace subsonic {

/// @brief Represents a Subsonic song object with metadata and streaming capabilities.
struct Song : public SubsonicObject {
	/// @brief A unique identifier for the song.
	String id;

	/// @brief The identifier of the parent directory or collection.
	String parent;

	/// @brief Indicates whether the object is a directory.
	bool isDir;

	/// @brief The title of the song.
	String title;

	/// @brief The name of the album that the song belongs to.
	String album;

	/// @brief The number of times the song has been played.
	int playCount;

	/// TODO created (datetime)

	unsigned long size;
	String contentType;
	String suffix;
	int duration;
	int bitRate;
	String path;
	bool isVideo;
	String albumId;
	String type;
	optional<String> coverArt;
	optional<String> artist;
	optional<String> artistId;
	optional<int> track;
	optional<int> year;
	optional<String> genre;
	optional<int> discNumber;
	optional<String> transcodedContentType;
	optional<String> transcodedSuffix;
	optional<float> averageRating;

	String uri() const;
};

} // namespace subsonic
