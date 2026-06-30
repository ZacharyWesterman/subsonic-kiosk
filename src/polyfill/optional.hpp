///@file optional.hpp

#pragma once

// Polyfill our own implementation of optional, since:
// 1. The std version is not supported before C++17
// 2. Exceptions are overhead we don't want.

#include <utility>

/// @brief The empty optional type.
struct nullopt_t {
	/**
	 * @brief Construct an empty optional value.
	 * @param value A dummy value.
	 */
	explicit constexpr nullopt_t(int value) noexcept {}
};

/// @brief The canonical empty optional value.
constexpr nullopt_t nullopt{0};

/**
 * @brief Safely handle data that may or may not have a value.
 * @tparam T the data type to store when there's a value.
 */
template <typename T>
class optional {
	union {
		char dummy;
		T val;
	};
	bool engaged;

public:
	/**
	 * @brief Construct an optional with an empty value.
	 *
	 * This allows for syntax like `optional var;`.
	 */
	constexpr optional() noexcept : dummy(0), engaged(false) {}

	/**
	 * @brief Construct an optional with an empty value.
	 *
	 * This allows for syntax like `optional var {};` or `return {};`.
	 *
	 * @param opt The empty value.
	 */
	constexpr optional(nullopt_t opt) noexcept : dummy(0), engaged(false) {}

	/**
	 * @brief Construct an optional from another optional.
	 * @param other The optional to copy from.
	 */
	optional(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	};

	/**
	 * @brief Construct an optional from another (temporary) optional.
	 * @param other The optional to copy from.
	 */
	optional(optional &&other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	}

	/**
	 * @brief Construct an optional with a non-empty value from temporary datas.
	 * @param u The value to store.
	 */
	optional(T &&u) : val(std::forward<T>(u)), engaged(true) {}

	/**
	 * @brief Construct an optional with a non-empty value.
	 * @param t The value to store.
	 */
	optional(const T &t) : val(t), engaged(true) {}

	/**
	 * @brief Assign an optional's data to this.
	 * @param other The optional to copy from.
	 * @return This after reassigning.
	 */
	optional &operator=(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
		return *this;
	}

	/**
	 * @brief Assign an optional's data to new non-empty data.
	 * @param other The data to assign.
	 * @return This after reassigning.
	 */
	optional &operator=(const T &other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	};

	/**
	 * @brief Assign an optional's data to new non-empty (temporary) data.
	 * @param other The data to assign.
	 * @return This after reassigning.
	 */
	optional &operator=(T &&other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	}

	/// @brief Destruct any contained data.
	~optional() {
		if (engaged) {
			val.~T();
		}
	}

	/**
	 * @brief Check if this optional contains any data.
	 * @return True if non-empty, false if empty.
	 * @see has_value()
	 */
	constexpr operator bool() const {
		return engaged;
	}

	/**
	 * @brief Check if this optional contains any data.
	 * @return True if non-empty, false if empty.
	 * @see operator bool()
	 */
	constexpr bool has_value() const {
		return engaged;
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see value()
	 * @see value_or()
	 */
	constexpr T &operator*() & {
		return val;
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see value()
	 * @see value_or()
	 */
	constexpr T &&operator*() && {
		return std::move(val);
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see operator*()
	 * @see value_or()
	 */
	T &value() & {
		return val;
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see operator*()
	 * @see value_or()
	 */
	const T &value() const & {
		return val;
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see operator*()
	 * @see value_or()
	 */
	T &&value() && {
		return std::move(val);
	}

	/**
	 * @brief Get the contained value.
	 * @return The contained value.
	 * @warning Calling this may result in odd behavior or crashes if this optional is empty! Always check with has_value() beforehand!
	 * @see operator*()
	 * @see value_or()
	 */
	const T &&value() const && {
		return std::move(val);
	}

	/**
	 * @brief Get the contained value if non-empty, or a fallback value if empty.
	 * @param fallback The fallback value to return if empty.
	 * @return The contained value, or the fallback if non-empty.
	 * @note This method is safer than value(), and should always be preferred when possible.
	 */
	template <class U>
	T value_or(U &&fallback) const {
		return engaged ? val : static_cast<T>(std::forward<U>(fallback));
	}
};
