#include "album.hpp"
#include "../../emulation_helpers.hpp"
#include "../client.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

Album::Album(const Client *client, int id, String &&name, String &&artist, String &&coverArt, optional<int> &&year, optional<int> &&averageRating, int playCount, optional<std::vector<Song>> &&songList) : client(client), id(id), name(name), artist(artist), coverArt(coverArt), year(year), averageRating(averageRating), playCount(playCount), songList(songList) {}

const std::vector<Song> &Album::songs() {
	if (!songList.has_value()) {
		// Fetch list of songs if not already fetched.
		songList = client->albumSongs(id).await().value_or<std::vector<Song>>({});
	}

	return songList.value();
}

template <>
optional<Album> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	return (Album{
		client,
		json_to(String, document["id"]).toInt(),
		json_key_to_or(String, document, "album", ""),
		json_key_to_or(String, document, "artist", ""),
		json_key_to_or(String, document, "coverArt", ""),
		json_optional_key_to(int, document, "year"),
		json_optional_key_to(int, document, "averageRating"),
		json_to_int(document["playCount"]),
		(json_contains_key(document, "song") ? jsonDecode<std::vector<Song>>(document["song"], client) : optional<std::vector<Song>>{}),
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

template <>
optional<std::vector<Album>> Response<std::vector<Album>>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	return jsonDecode<std::vector<Album>>(json["subsonic-response"]["directory"]["child"], client);
}

} // namespace subsonic
