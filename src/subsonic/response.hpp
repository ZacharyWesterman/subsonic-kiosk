#pragma once
#include "../net/request.hpp"
#include "../polyfill/optional.hpp"

namespace subsonic {

class Client;

template <typename T>
class Response {
	net::Request requestData;
	const Client *client;

public:
	Response(net::Request &&request, const Client *client) : requestData(request), client(client) {}

	inline net::Request &request() {
		return requestData;
	}

	inline bool ready() const {
		return requestData.done();
	}
	inline operator bool() const {
		return requestData.ok();
	}
	inline bool ok() const {
		return requestData.ok();
	}
	inline void process() {
		requestData.process();
	}

	// This needs to be implemented for each object!
	optional<T> await();
};

} // namespace subsonic
