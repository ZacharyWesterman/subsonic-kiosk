#pragma once
#include "../net/request.hpp"
#include "../polyfill/optional.hpp"

namespace subsonic {

template <typename T>
class Response {
	net::Request requestData;

public:
	Response(net::Request &&request) : requestData(request) {}

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
