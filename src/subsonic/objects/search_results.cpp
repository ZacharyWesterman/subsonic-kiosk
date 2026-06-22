#include "search_results.hpp"
#include "../../emulation_helpers.hpp"
#include "../client.hpp"
#include "playlist.hpp"
#include <iostream>
#include <vector>

namespace subsonic {

template <>
optional<SearchResults> Response<SearchResults>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	auto &results = json["subsonic-response"]["searchResult2"];

	if (!json_is_obj(results)) {
		return {};
	}

	std::vector<Artist> artists;
	std::vector<Album> albums;
	std::vector<Song> songs;

	if (json_is_array(results["song"])) {
		auto list = json_to(JsonArray, results["song"]);
		songs.reserve(list.size());
		for (auto item : list) {
			auto song = jsonDecode<Song>(item);
			if (song.has_value()) {
				songs.push_back(song.value());
			}
		}
	}

	return SearchResults{
		artists,
		albums,
		songs,
	};
}

} // namespace subsonic
