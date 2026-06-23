#include "playlist.hpp"
#include "../../emulation_helpers.hpp"
#include "../client.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

Playlist::Playlist(const Client *client, int id, String &&name, String &&comment, String &&owner, String &&coverArt, int songCount, int duration, int isPublic) : client(client), id(id), name(name), comment(comment), owner(owner), coverArt(coverArt), songCount(songCount), duration(duration), isPublic(isPublic) {}

Response<std::vector<Song>> Playlist::songs() {
	return Response<std::vector<Song>>(client->query("getPlaylist", String("id=") + id), client);
}

template <>
optional<Playlist> jsonDecode(const JsonDocument &document, const Client *client) {
	if (!json_is_obj(document)) {
		return {};
	}

	auto item = json_to(JsonObject, document);

	return (Playlist{
		client,
		json_to(String, item["id"]).toInt(),
		json_to(String, item["name"]),
		json_to(String, item["comment"]),
		json_to(String, item["owner"]),
		json_to(String, item["coverArt"]),
		json_to(int, item["songCount"]),
		json_to(int, item["duration"]),
		json_to(bool, item["public"]),
	});
}

template <>
optional<std::vector<Playlist>> Response<std::vector<Playlist>>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	auto arr = json_to(JsonArray, json["subsonic-response"]["playlists"]["playlist"]);
	std::vector<Playlist> results;
	results.reserve(arr.size());

	for (auto item : arr) {
		auto playlist = jsonDecode<Playlist>(item, client);
		if (playlist.has_value()) {
			results.push_back(playlist.value());
		}
	}

	return results;
}

template <>
optional<Playlist> Response<Playlist>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	return jsonDecode<Playlist>(json["subsonic-response"]["playlist"], client);
}

} // namespace subsonic
