#pragma once

#include "../../polyfill/optional.hpp"
#include "song.hpp"
#include "subsonic_object.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

/// @brief Represents a music album with metadata and related operations.
struct Album : public SubsonicObject {

	String id;
	bool isDir;
	String title;
	String album;
	int playCount;
	/// TODO created (datetime)

	optional<String> artist;
	optional<int> year;
	optional<String> genre;
	optional<String> coverArt;
	optional<float> averageRating;
	optional<String> parent;

	const std::vector<Song> &songs() const;
	const String &cover() const;

private:
	optional<std::vector<Song>> songList;
};

} // namespace subsonic
