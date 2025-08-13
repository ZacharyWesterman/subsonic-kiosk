#pragma once

#include <functional>

namespace callback {

class repeat {
	unsigned long lastTime;
	unsigned long interval;
	std::function<void()> func;

public:
	repeat(unsigned long interval, std::function<void()> func);

	void operator()();
};

} // namespace callback