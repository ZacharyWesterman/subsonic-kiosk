#pragma once

#include "album.hpp"
#include "artist.hpp"
#include "song.hpp"
#include <vector>

namespace subsonic {

struct SearchResults {
	std::vector<Artist> artists;
	std::vector<Album> albums;
	std::vector<Song> songs;
};

} // namespace subsonic
