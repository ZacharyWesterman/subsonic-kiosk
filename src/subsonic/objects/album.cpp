#include "album.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<Album> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	return (Album{
		(json_contains_key(document, "song") ? jsonDecode<std::vector<Song>>(document["song"], client) : optional<std::vector<Song>>{}),
		json_to(String, document["id"]).toInt(),
		json_key_to_or(String, document, "album", ""),
		json_key_to_or(String, document, "artist", ""),
		json_key_to_or(String, document, "coverArt", ""),
		json_optional_key_to(int, document, "year"),
		json_optional_key_to(int, document, "averageRating"),
		json_to_int(document["playCount"]),
	});
}

template <>
optional<std::vector<Album>> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_array(document)) {
		return {};
	}

	std::vector<Album> albums;
	auto data = json_to_array(document);
	albums.reserve(data.size());

	for (auto item : data) {
		auto album = jsonDecode<Album>(item, client);
		if (album.has_value()) {
			albums.push_back(album.value());
		}
	}

	return albums;
}

template <>
optional<Album> Response<Album>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	return jsonDecode<Album>(json["subsonic-response"]["album"], client);
}

} // namespace subsonic
