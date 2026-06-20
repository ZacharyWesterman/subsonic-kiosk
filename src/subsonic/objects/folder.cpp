#include "folder.hpp"
#include "../../emulation_helpers.hpp"
#include "../response.hpp"
#include <vector>

namespace subsonic {

template <>
optional<std::vector<Folder>> Response<std::vector<Folder>>::await() {
	if (!requestData.ok()) {
		return {};
	}

	auto json = requestData.json();

	if (json["subsonic-response"]["status"] != "ok") {
		return {};
	}

	std::vector<Folder> folders;
	auto arr = json_to(JsonArray, json["subsonic-response"]["musicFolders"]["musicFolder"]);
	for (auto item : arr) {
		folders.push_back(Folder{
			json_to(String, item["name"]),
			json_to(int, item["id"]),
		});
	}

	return folders;
}

} // namespace subsonic
