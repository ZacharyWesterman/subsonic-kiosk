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

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	return Ping{
		json_to_str(json["subsonic-response"]["status"]),
		json_to_str(json["subsonic-response"]["version"]),
		json_to_str(json["subsonic-response"]["type"]),
	};
}

} // namespace subsonic
