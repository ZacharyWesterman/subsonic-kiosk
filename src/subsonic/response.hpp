/// @file response.hpp
#pragma once
#include "../net/request.hpp"
#include "../polyfill/optional.hpp"
#include <ArduinoJson.h>

namespace subsonic {

class Client;
/**
 * @brief This class represents a deferred response to any subsonic query.
 *
 * The client is queried, and when needed (or when ready), the data can be awaited.
 * The request text is only actually parsed when await() is called.
 */
template <typename T>
class Response {
	net::Request requestData;
	const Client *client;

public:
	/**
	 * @brief Construct a deferred response.
	 * @param request The request associated with this response.
	 * @param client The client that may be passed to the resolved object.
	 */
	Response(net::Request &&request, const Client *client) : requestData(request), client(client) {}

	/**
	 * @brief Get the actual request object.
	 * @return The request object.
	 */
	inline net::Request &request() {
		return requestData;
	}

	/**
	 * @brief Check if the request has finished.
	 * @return True if finished, false otherwise.
	 */
	inline bool ready() const {
		return requestData.done();
	}

	/**
	 * @brief Check if the request is good.
	 * @return True if the request is good, false otherwise.
	 */
	inline operator bool() const {
		return requestData.ok();
	}

	/**
	 * @brief Check if the request is good.
	 * @return True if the request is good, false otherwise.
	 */
	inline bool ok() const {
		return requestData.ok();
	}

	/// @brief Process any more of the request as needed.
	inline void process() {
		requestData.process();
	}

	/**
	 * @brief Wait for the request to finish, then parse the response and (if valid) return it.
	 * @return An optional containing the response object if valid, or nothing if invalid.
	 * @note When developing, a template specialization for every possible `T` must be implemented.
	 */
	optional<T> await();
};

/**
 * @brief Decode a JSON document into a specific Subsonic response object.
 * @param json The JSON document to decode.
 * @param client The Subsonic client. This can allow for features like streaming or deferred requests.
 * @return An optional containing the response object if valid, or nothing if invalid.
 * @note When developing, a template specialization for every possible `T` must be implemented.
 */
template <typename T>
optional<T> jsonDecode(const JsonDocument &json, const Client *client);

} // namespace subsonic
