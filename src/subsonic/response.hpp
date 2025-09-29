#pragma once
#include "../net/request.hpp"

namespace subsonic {

class Response {
	net::Request request;
	bool fetchedData = false;
	JsonDocument responseJson;

	void parseData();

public:
	Response(net::Request &&request);

	bool done() const;

	inline bool errored() {
		return !ok();
	}
	bool ok();

	String error();

	String text();
	JsonDocument json();
};

} // namespace subsonic
