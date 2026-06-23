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
		json_to(String, item["name"]),
		json_to_or(String, item["artist"], ""),
		json_to_or(String, item["coverArt"], ""),
		json_optional_to(int, item["year"]),
		json_optional_to(int, item["averageRating"]),
		json_to_int(item["playCount"]),
	});
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
