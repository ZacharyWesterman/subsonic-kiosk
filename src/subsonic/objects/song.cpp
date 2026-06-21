#include "song.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<std::vector<Song>> Response<std::vector<Song>>::await() {
	// Two possible ways to get a list of songs:
	// 1. Querying from a playlist.
	// 2. Querying from an album.

	if (!requestData.ok()) {
		return {};
	}

	return {};
}

} // namespace subsonic
