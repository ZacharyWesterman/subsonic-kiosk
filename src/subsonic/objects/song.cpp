#include "song.hpp"
#include "../../emulation_helpers.hpp"
#include "../../fs.hpp"
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

	// Serial.println(requestData.text());
	// fs::Path("/songs-output.json").write(requestData.text());
	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	std::vector<Song> results;
	JsonArray data;

	if (json_is_array(json["subsonic-response"]["playlist"])) {
		// Querying from a playlist.
		data = json_to(JsonArray, json["subsonic-response"]["playlist"]["entry"]);
	}

	results.reserve(data.size());
	for (auto item : data) {
		// Only songs here!
		if (json_to(bool, item["isDir"]) || json_to(bool, item["isVideo"])) {
			continue;
		}

		results.push_back(Song{
			json_to(String, item["id"]).toInt(),
			json_to(String, item["parent"]).toInt(),
			json_to(String, item["title"]),
			json_to(String, item["album"]),
			json_to(String, item["contentType"]),
			json_to(String, item["suffix"]),
			json_to(String, item["path"]),
			json_to(String, item["type"]),
			json_to(int, item["playCount"]),
			json_to(unsigned long, item["size"]),
			json_to(int, item["duration"]),
			json_to(String, item["albumId"]).toInt(),
		});
	}

	return results;
}

} // namespace subsonic
