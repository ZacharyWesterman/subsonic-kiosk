#include "netClient.hpp"

namespace net {

NetClient::NetClient(const String &host, int port) : host(host), port(port) {
#ifdef EMULATE
	connected = true;
#else
	connected = client.connect(host.c_str(), port);
#endif
}

NetClient::~NetClient() {
#ifndef EMULATE
	if (connected) {
		client.stop();
	}
#endif
}

Request NetClient::get(const String &path, unsigned long timeout) {
#ifdef EMULATE
	String requestPath = path;
	if (!requestPath.startsWith("/")) {
		requestPath = "/" + requestPath;
	}
	const String scheme = (port == 443) ? "https" : "http";
	const String requestUrl = scheme + "://" + host + ":" + String(port) + requestPath;
	return Request(requestUrl, timeout);
#else
	if (!connected) {
		connected = client.connect(host.c_str(), port);
	}

	if (!connected) {
		return Request(client, timeout);
	}

	client.println("GET " + path + " HTTP/1.1");
	client.println("Host: " + host);
	client.println("Connection: close");
	client.println();

	Request response(client, timeout);
	return response;
#endif
}

} // namespace net
