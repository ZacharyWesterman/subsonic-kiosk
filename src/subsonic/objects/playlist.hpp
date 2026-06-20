#pragma once

#include "../../polyfill/optional.hpp"
#include "song.hpp"
#include "subsonic_object.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

/// @brief Represents a Subsonic playlist with netadata and song entries.

struct Playlist : public SubsonicObject {
	/// @brief A unique identifer for this playlist.
	String id;

	///@brief The name of the playlist.
	String name;

	///@brief Any comment associated with this playlist (usually a description).
	String comment;

	///@brief The username of the playlist owner.
	String owner;

	///@todo Creation timestamp (datetime)
	///@todo Changed timestamp (datetime)

	///@brief The cover art of the playlist.
	String coverArt;

	/// @brief The number of songs in the playlist.
	int songCount;

	///@brief The total duration of the playlist in seconds.
	int duration;

	///@brief Indicates if the playlist is public.
	bool public;

	optional<std::vector<Song>> songs();

private:
	/// @brief A lost of songs in the playlist, if any.
	optional<std::vector<Song>> entry;
}

} // namespace subsonic
