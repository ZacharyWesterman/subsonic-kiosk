#include "song.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<Song> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	// Only keep if it's actually a song!
	if (json_to(bool, document["isDir"]) || json_to(bool, document["isVideo"])) {
		return {};
	}

	return (Song{
		json_to(String, document["id"]).toInt(),
		json_to(String, document["parent"]).toInt(),
		json_to(String, document["title"]),
		json_to(String, document["album"]),
		json_to_or(String, document["artist"], ""),
		json_to(String, document["contentType"]),
		json_to(String, document["suffix"]),
		json_to(String, document["path"]),
		json_to(int, document["playCount"]),
		json_to(unsigned long, document["size"]),
		json_to(int, document["duration"]),
		json_to(String, document["albumId"]).toInt(),
		json_optional_key_to(int, document, "track"),
		json_optional_key_to(int, document, "year"),
		json_optional_key_to(int, document, "discNumber"),
		json_optional_key_to(int, document, "averageRating"),
	});
}

template <>
optional<std::vector<Song>> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_array(document)) {
		return {};
	}

	std::vector<Song> songs;
	auto data = json_to_array(document);
	songs.reserve(data.size());

	for (auto item : data) {
		auto song = jsonDecode<Song>(item, client);
		if (song.has_value()) {
			songs.push_back(song.value());
		}
	}

	return songs;
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

	if (json_contains_key(json["subsonic-response"], "playlist")) {
		// Querying from a playlist
		return jsonDecode<std::vector<Song>>(json["subsonic-response"]["playlist"]["entry"], client);
	}

	// Querying from an album
	return jsonDecode<std::vector<Song>>(json["subsonic-response"]["directory"]["child"], client);
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

	auto song = jsonDecode<Song>(json["subsonic-response"]["song"], client);
	if (!song.has_value()) {
		return {};
	} else {
		return song.value();
	}
}

} // namespace subsonic
