#pragma once
#include "objects/folder.hpp"
#include "objects/ping.hpp"
#include "objects/playlist.hpp"
#include "objects/search_results.hpp"
#include "response.hpp"
#include <vector>

namespace subsonic {

class Client {
	String host;
	String user;
	String md5sum;
	String salt;

public:
	Client(const String &host, const String &user, const String &pass_md5, const String &salt);
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
};

} // namespace subsonic
