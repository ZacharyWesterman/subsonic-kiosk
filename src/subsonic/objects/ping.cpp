#include "ping.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"

namespace subsonic {

template <>
optional<Ping> Response<Ping>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();
	const auto response = json["subsonic-response"];

	if (response["status"] != "ok") {
		return {};
	}

	return Ping{json_to_str(response["status"]), json_to_str(response["version"]), json_to_str(response["type"])};
}

} // namespace subsonic
