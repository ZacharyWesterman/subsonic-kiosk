#include "client.hpp"
#include "../net.hpp"

namespace subsonic {

Client::Client(const String &host, const String &user, const String &pass_md5, const String &salt) : host(host), user(user), md5sum(pass_md5), salt(salt) {}

Client::~Client() {}

net::Request Client::query(const String &action, const String &parameters) const {
	String url = host + "/rest/" + action + ".view?u=" + user + "&t=" + md5sum + "&s=" + salt + "&v=1.15.1&c=subsonic-arduino&f=json";
	if (!parameters.isEmpty()) {
		url += "&" + parameters;
	}

	return net::get(url);
}

Response<Ping> Client::ping() const {
	return Response<Ping>(query("ping"), this);
}

Response<std::vector<Folder>> Client::folders() const {
	return Response<std::vector<Folder>>(query("getMusicFolders"), this);
}

Response<std::vector<Playlist>> Client::playlists() const {
	return Response<std::vector<Playlist>>(query("getPlaylists"), this);
}

Response<Playlist> Client::playlist(int id) const {
	return Response<Playlist>(query("getPlaylist", "id=" + String(id)), this);
}

Response<Song> Client::song(int id) const {
	return Response<Song>(query("getSong", "id=" + String(id)), this);
}

Response<Album> Client::album(int id) const {
	return Response<Album>(query("getAlbum", "id=" + String(id)), this);
}

Response<std::vector<Song>> Client::albumSongs(int albumId) const {
	return Response<std::vector<Song>>(query("getMusicDirectory", "id=" + String(albumId)), this);
}

Response<Artist> Client::artist(int id) const {
	return Response<Artist>(query("getMusicDirectory", "id=" + String(id)), this);
}

Response<std::vector<Album>> Client::artistAlbums(int artistId) const {
	return Response<std::vector<Album>>(query("getMusicDirectory", "id=" + String(artistId)), this);
}

Response<SearchResults> Client::search(const String &text) const {
	return Response<SearchResults>(query("search2", "query=" + net::urlencode(text)), this);
}

} // namespace subsonic
