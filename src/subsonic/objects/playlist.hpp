/// @file playlist.hpp
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
	/// @brief The Subsonic client used for deferred requests.
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

	/**
	 * @brief Construct this playlist with all fields.
	 * @param client The Subsonic client.
	 * @param id The playlist ID.
	 * @param name The playlist name.
	 * @param comment The playlist description.
	 * @param owner The playlist owner's username.
	 * @param coverArt The ID of the playlist's cover art.
	 * @param songCount The number of songs in the playlist.
	 * @param duration The total duration of the playlist in seconds.
	 * @param isPublic Whether the playlist is public or not.
	 */
	Playlist(const Client *client, int id, String &&name, String &&comment, String &&owner, String &&coverArt, int songCount, int duration, bool isPublic);

	/**
	 * @brief Get a list of all songs in the playlist.
	 * @return A deferred response that resolves to a list of songs.
	 * @warning This returns ALL songs (not paginated), so for very large playlists this may take a long time to complete!
	 */
	Response<std::vector<Song>> songs();
};

} // namespace subsonic
