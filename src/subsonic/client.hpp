/// @file client.hpp
#pragma once
#include "objects/album.hpp"
#include "objects/folder.hpp"
#include "objects/ping.hpp"
#include "objects/playlist.hpp"
#include "objects/search_results.hpp"
#include "objects/song.hpp"
#include "response.hpp"
#include <vector>

namespace subsonic {

/// @brief This class represents a client connection to a single Subsonic instance.
class Client {
	String host;
	String user;
	String md5sum;
	String salt;

public:
	/**
	 * @brief Construct a Subsonic client with all required information.
	 * @param host The host url, e.g. `http://example.com/airsonic`.
	 * @param user The username.
	 * @param pass_md5 The salted MD5sum of the user's password.
	 * @param salt The salt used when hashing the password.
	 */
	Client(const String &host, const String &user, const String &pass_md5, const String &salt);

	/// @brief The destructor.
	~Client();

	/**
	 * @brief Executes a query against the Subsonic API.
	 * @param action The API action to perform.
	 * @param parameters Parameters to include in the query, if any.
	 */
	net::Request query(const String &action, const String &parameters = "") const;

	/**
	 * @brief Ping the Subsonic API for a response.
	 * @return The ping response.
	 */
	Response<Ping> ping() const;

	/**
	 * @brief Get a list of available folders.
	 * @return The folder list response.
	 */
	Response<std::vector<Folder>> folders() const;

	/**
	 * @brief Get all available playlists
	 * @return A list of playlists.
	 */
	Response<std::vector<Playlist>> playlists() const;

	/**
	 * @brief Get a playlist by its ID.
	 * @param id The ID of the playlist.
	 * @return A response containing the playlist if it exists.
	 */
	Response<Playlist> playlist(int id) const;

	/**
	 * @brief Get a song by its ID.
	 * @param id The ID of the song.
	 * @return A response containing the song if it exists.
	 */
	Response<Song> song(int id) const;

	/**
	 * @brief Get an album by its ID.
	 * @param id The ID of the album.
	 * @return A response containing the album if it exists.
	 */
	Response<Album> album(int id) const;

	/**
	 * @brief Get the list of all songs in an album.
	 * @param albumId The ID of the album.
	 * @return A response containing the list of songs if the album exists.
	 */
	Response<std::vector<Song>> albumSongs(int albumId) const;

	/**
	 * @brief Get an artist by its ID.
	 * @param id The ID of the artist.
	 * @return A response containing the artist if it exists.
	 */
	Response<Artist> artist(int id) const;

	/**
	 * @brief Get the list of all albums by an artist.
	 * @param artistId The ID of the artist.
	 * @return A response containing the list of albums if the artist exists.
	 */
	Response<std::vector<Album>> artistAlbums(int artistId) const;

	/**
	 * @brief Search for albums, songs or artists.
	 * @param text The text to search for. This may be title, description, etc.
	 * @return A response that resolves to the search results.
	 */
	Response<SearchResults> search(const String &text) const;
};

} // namespace subsonic
