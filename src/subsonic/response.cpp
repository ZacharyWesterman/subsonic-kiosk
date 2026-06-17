#include "response.hpp"

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
	if (responseJson.isNull()) {
		return false;
	}
	return responseJson["subsonic-response"]["status"].as<String>() == "ok";
}

String Response::error() {
	if (!request.ok()) {
		return String("HTTP request returned error code ") + String(static_cast<int>(request.status())) + ".";
	}

	parseData();
	if (responseJson.isNull()) {
		return "No data received.";
	}
	return responseJson["subsonic-response"]["error"]["message"].as<String>();
}

String Response::text() {
	return request.text();
}

JsonDocument Response::json() {
	return request.json();
}

} // namespace subsonic
