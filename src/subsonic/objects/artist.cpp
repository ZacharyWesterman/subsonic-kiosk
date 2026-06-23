#include "artist.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<Artist> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	auto item = json_to(JsonObject, document);

	return (Artist{
		json_to(String, item["id"]).toInt(),
		json_to(String, item["name"]),
	});
}

template <>
optional<std::vector<Artist>> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_array(document)) {
		return {};
	}

	std::vector<Artist> artists;
	auto data = json_to(JsonArray, document);
	artists.reserve(data.size());

	for (auto item : data) {
		auto artist = jsonDecode<Artist>(item, client);
		if (artist.has_value()) {
			artists.push_back(artist.value());
		}
	}

	return artists;
}

template <>
optional<Artist> Response<Artist>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	return jsonDecode<Artist>(json["subsonic-response"]["artist"], client);
}

} // namespace subsonic
