#include "response.hpp"
#include "../emulation_helpers.hpp"

namespace subsonic {

Response::Response(net::Request &&request) : request(request) {}

void Response::parseData() {
	if (fetchedData) {
		return;
	}

	fetchedData = true;
	responseJson = request.json();
}

bool Response::done() const {
	return request.done();
}

bool Response::ok() {
	if (!request.ok()) {
		return false;
	}

	parseData();
	if (json_is_null(responseJson)) {
		return false;
	}
	return json_to_str(responseJson["subsonic-response"]["status"]) == "ok";
}

String Response::error() {
	if (!request.ok()) {
		return String("HTTP request returned error code ") + static_cast<int>(request.status()) + ".";
	}

	parseData();
	if (json_is_null(responseJson)) {
		return "No data received.";
	}
	return json_to_str(responseJson["subsonic-response"]["error"]["message"]);
}

String Response::text() {
	return request.text();
}

JsonDocument Response::json() {
	return request.json();
}

} // namespace subsonic
