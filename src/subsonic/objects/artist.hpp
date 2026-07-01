/// @file artist.hpp
#pragma once

#include "../../polyfill/optional.hpp"
#include "album.hpp"
#include <Arduino.h>
#include <vector>

namespace subsonic {

class Client;

/// @brief Represents a music artist.
struct Artist {
	/// @brief The Subsonic client used for deferred requests.
	const Client *client;

	/// @brief A unique identifier for the artist.
	int id;

	/// @brief The name of the artist.
	String name;

	/**
	 * @brief Construct this artist with all fields.
	 * @param client The Subsonic client.
	 * @param id The artist ID.
	 * @param name The artist name.
	 */
	Artist(const Client *client, int id, String &&name);

	/**
	 * @brief Get a list of all albums by the artist.
	 * @return A list of all albums by the artist.
	 * @note If the albums have not already been fetched, this will make
	 * a network request and wait until the response is parsed. Future calls will then
	 * use this cached data.
	 */
	std::vector<Album> &albums();

private:
	optional<std::vector<Album>> albumList;
};

} // namespace subsonic
