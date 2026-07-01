#include "artist.hpp"
#include "../../emulation_helpers.hpp"
#include "../client.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

Artist::Artist(const Client *client, int id, String &&name) : client(client), id(id), name(name) {}

std::vector<Album> &Artist::albums() {
	if (!albumList.has_value()) {
		// Fetch list of albums if not already fetched.
		albumList = client->artistAlbums(id).await().value_or<std::vector<Album>>({});
	}

	return albumList.value();
}

template <>
optional<Artist> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	return (Artist{
		client,
		json_to(String, document["id"]).toInt(),
		json_to(String, document["name"]),
	});
}

template <>
optional<std::vector<Artist>> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_array(document)) {
		return {};
	}

	std::vector<Artist> artists;
	auto data = json_to_array(document);
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

	return jsonDecode<Artist>(json["subsonic-response"]["directory"], client);
}

} // namespace subsonic
