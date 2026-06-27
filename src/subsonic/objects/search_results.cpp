#include "search_results.hpp"
#include "../../emulation_helpers.hpp"
#include "../client.hpp"
#include "playlist.hpp"
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

	JsonDocument results = json["subsonic-response"]["searchResult2"];

	if (!json_is_obj(results)) {
		return {};
	}

	return SearchResults{
		jsonDecode<std::vector<Artist>>(results["artist"], client).value_or<std::vector<Artist>>({}),
		jsonDecode<std::vector<Album>>(results["album"], client).value_or<std::vector<Album>>({}),
		jsonDecode<std::vector<Song>>(results["song"], client).value_or<std::vector<Song>>({}),
	};
}

} // namespace subsonic
