#include "song.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<Song> jsonDecode(const JsonDocument &document) {
	if (!json_is_obj(document)) {
		return {};
	}

	auto item = json_to(JsonObject, document);

	// Only keep if it's actually a song!
	if (json_to(bool, item["isDir"]) || json_to(bool, item["isVideo"])) {
		return {};
	}

	return (Song{
		json_to(String, item["id"]).toInt(),
		json_to(String, item["parent"]).toInt(),
		json_to(String, item["title"]),
		json_to(String, item["album"]),
		json_to_or(String, item["artist"], ""),
		json_to(String, item["contentType"]),
		json_to(String, item["suffix"]),
		json_to(String, item["path"]),
		json_to(String, item["type"]),
		json_to(int, item["playCount"]),
		json_to(unsigned long, item["size"]),
		json_to(int, item["duration"]),
		json_to(String, item["albumId"]).toInt(),
		json_optional_to(int, item["track"]),
		json_optional_to(int, item["year"]),
		json_optional_to(int, item["discNumber"]),
		json_optional_to(int, item["averageRating"]),
	});
}

template <>
optional<std::vector<Song>> Response<std::vector<Song>>::await() {
	// Two possible ways to get a list of songs:
	// 1. Querying from a playlist.
	// 2. Querying from an album.

	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	std::vector<Song> results;
	JsonArray data;

	if (json_is_obj(json["subsonic-response"]["playlist"])) {
		// Querying from a playlist.
		data = json_to(JsonArray, json["subsonic-response"]["playlist"]["entry"]);
	}

	results.reserve(data.size());
	for (auto item : data) {
		auto song = jsonDecode<Song>(item);
		if (song.has_value()) {
			results.push_back(song.value());
		}
	}

	return results;
}

template <>
optional<Song> Response<Song>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	auto song = jsonDecode<Song>(json["subsonic-response"]["song"]);
	if (!song.has_value()) {
		return {};
	} else {
		return song.value();
	}
}

} // namespace subsonic
