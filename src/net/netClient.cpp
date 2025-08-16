#include "netClient.hpp"

namespace net {

NetClient::NetClient(const String &host, int port) : host(host), port(port) {
	connected = client.connect(host.c_str(), port);
}

NetClient::~NetClient() {
	if (connected) {
		client.stop();
	}
}

Request NetClient::get(const String &path) {
	if (!connected) {
		connected = client.connect(host.c_str(), port);
	}

	if (!connected) {
		return Request(client);
	}

	client.println("GET " + path + " HTTP/1.1");
	client.println("Host: " + host);
	client.println("Connection: close");
	client.println();

	Request response(client);
	return response;
}

} // namespace net
