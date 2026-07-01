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

	Artist(const Client *client, int id, String &&name);

	std::vector<Album> &albums();

private:
	optional<std::vector<Album>> albumList;
};

} // namespace subsonic
