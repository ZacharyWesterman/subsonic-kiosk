#pragma once

#include "../client.hpp"

namespace subsonic {

class Client;

struct SubsonicObject {
	const Client &client;
};

} // namespace subsonic
