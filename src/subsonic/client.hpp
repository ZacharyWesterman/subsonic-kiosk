#pragma once
#include "objects/search_results.hpp"
#include "response.hpp"

namespace subsonic {

class Client {
	String host;
	String user;
	String md5sum;
	String salt;

public:
	Client(const String &host, const String &user, const String &pass_md5, const String &salt);
	~Client();

	/**
	 * @brief Executes a query against the Subsonic API.
	 * @param action The API action to perform.
	 * @param parameters Parameters to include in the query, if any.
	 */
	Response query(const String &action, const String &parameters) const;
};

} // namespace subsonic
