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

	std::vector<Song> songs;
	if (json_is_array(item["song"])) {
		auto songList = json_to(JsonArray, item["song"]);
		songs.reserve(songList.size());

		for (auto song : songList) {
			// Only keep if it's actually a song!
			if (json_to(bool, song["isDir"]) || json_to(bool, song["isVideo"])) {
				return {};
			}

			songs.push_back(Song{
				json_to(String, song["id"]).toInt(),
				json_to(String, song["parent"]).toInt(),
				json_to(String, song["title"]),
				json_to(String, song["album"]),
				json_to(String, song["contentType"]),
				json_to(String, song["suffix"]),
				json_to(String, song["path"]),
				json_to(String, song["type"]),
				json_to(int, song["playCount"]),
				json_to(unsigned long, song["size"]),
				json_to(int, song["duration"]),
				json_to(String, song["albumId"]).toInt(),
				json_optional_to(int, song["track"]),
				json_optional_to(int, song["year"]),
				json_optional_to(int, song["discNumber"]),
				json_optional_to(float, song["averageRating"]),
			});
		}
	}

	return (Album{
		songs,
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
