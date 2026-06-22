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

} // namespace subsonic
