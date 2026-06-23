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

	auto item = json_to(JsonObject, document);

	return (Album{
		jsonDecode<std::vector<Song>>(item["song"], client),
		json_to(String, item["id"]).toInt(),
		json_to_or(String, item["album"], ""),
		json_to_or(String, item["artist"], ""),
		json_to_or(String, item["coverArt"], ""),
		json_optional_to(int, item["year"]),
		json_optional_to(int, item["averageRating"]),
		json_to_int(item["playCount"]),
	});
}

template <>
optional<std::vector<Album>> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_array(document)) {
		return {};
	}

	std::vector<Album> albums;
	auto data = json_to(JsonArray, document);
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
