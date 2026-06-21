#pragma once

#include "../../polyfill/optional.hpp"
#include "../response.hpp"
#include "song.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

class Client;

/// @brief Represents a Subsonic playlist with netadata and song entries.
struct Playlist {
	const Client *client;

	///@brief A unique identifer for this playlist.
	int id;

	///@brief The name of the playlist.
	String name;

	///@brief Any comment associated with this playlist (usually a description).
	String comment;

	///@brief The username of the playlist owner.
	String owner;

	///@todo Creation timestamp (datetime)
	///@todo Changed timestamp (datetime)

	///@brief The ID of the playlist's cover art
	String coverArt;

	/// @brief The number of songs in the playlist.
	int songCount;

	///@brief The total duration of the playlist in seconds.
	int duration;

	///@brief Indicates if the playlist is public.
	bool isPublic;

	Playlist(const Client *client, int id, String &&name, String &&comment, String &&owner, String &&coverArt, int songCount, int duration, int isPublic);

	Response<std::vector<Song>> songs();
};

} // namespace subsonic
