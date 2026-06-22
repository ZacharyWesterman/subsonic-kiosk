#include "album.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <iostream>
#include <vector>

namespace subsonic {

template <>
optional<Album> Response<Album>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}
	if (!json_is_obj(json["subsonic-response"]["album"])) {
		return {};
	}

	auto item = json_to(JsonObject, json["subsonic-response"]["album"]);

	std::cout << item << std::endl;

	return (Album{
		std::vector<Song>{},
		json_to(String, item["id"]).toInt(),
		json_to(String, item["name"]),
		json_to_or(String, item["artist"], ""),
		json_to_or(String, item["coverArt"], ""),
		json_optional_to(int, item["year"]),
		json_optional_to(float, item["averageRating"]),
		json_to_int(item["playCount"]),
	});
}

} // namespace subsonic
