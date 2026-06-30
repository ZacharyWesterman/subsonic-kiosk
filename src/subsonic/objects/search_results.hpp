#pragma once

#include "album.hpp"
#include "artist.hpp"
#include "song.hpp"
#include <vector>

namespace subsonic {

/// @brief This class represents the results of a Subsonic search.
struct SearchResults {

	/// @brief The list of artists returned from the search.
	std::vector<Artist> artists;

	/// @brief The list of albums returned from the search.
	std::vector<Album> albums;

	/// @brief The list of songs returned from the search.
	std::vector<Song> songs;
};

} // namespace subsonic
