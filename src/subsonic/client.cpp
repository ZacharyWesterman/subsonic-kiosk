#include "client.hpp"
#include "../net.hpp"

namespace subsonic {

Client::Client(const String &host, const String &user, const String &pass_md5, const String &salt) : host(host), user(user), md5sum(pass_md5), salt(salt) {}

Client::~Client() {}

Response Client::query(const String &action, const String &parameters) const {
	String url = host + "/rest/" + action + ".view?u=" + user + "&t=" + md5sum + "&s=" + salt + "&v=1.15.1&c=subsonic-arduino&f=json";
	if (!parameters.isEmpty()) {
		url += "&" + parameters;
	}

	return net::get(url);
}

} // namespace subsonic
