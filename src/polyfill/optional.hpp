#pragma once

// Polyfill our own implementation of optional, since:
// 1. The std version is not supported before C++17
// 2. Exceptions are overhead we don't want.

#if !defined(DOXYGEN)
#include <utility>

struct nullopt_t {
	explicit constexpr nullopt_t(int) noexcept {}
};
constexpr nullopt_t nullopt{0};

template <typename T>
class optional {
	union {
		char dummy;
		T val;
	};
	bool engaged;

public:
	constexpr optional() noexcept : dummy(0), engaged(false) {}
	constexpr optional(nullopt_t) noexcept : dummy(0), engaged(false) {}

	optional(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	};
	optional(optional &&other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	}
	optional(T &&u) : val(std::forward<T>(u)), engaged(true) {}

	optional(const T &t) : val(t), engaged(true) {}

	optional &operator=(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
		return *this;
	}
	optional &operator=(const T &other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	};
	optional &operator=(T &&other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	}

	~optional() {
		if (engaged) {
			val.~T();
		}
	}

	constexpr operator bool() const {
		return engaged;
	}
	constexpr bool has_value() const {
		return engaged;
	}

	constexpr T &operator*() & {
		return val;
	}
	// constexpr const T &operator*() const & {
	// 	return val;
	// }
	constexpr T &&operator*() && {
		return std::move(val);
	}
	// constexpr const T &&operator*() const && {
	// 	return std::move(val);
	// }

	T &value() & {
		return val;
	}
	const T &value() const & {
		return val;
	}
	T &&value() && {
		return std::move(val);
	}
	const T &&value() const && {
		return std::move(val);
	}

	template <class U>
	T value_or(U &&u) const {
		return engaged ? val : static_cast<T>(std::forward<U>(u));
	}
};

#endif
