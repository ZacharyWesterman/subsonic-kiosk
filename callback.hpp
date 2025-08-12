#pragma once

#include <functional>

namespace callback {

class repeat {
	unsigned long lastTime;
	unsigned long interval;
	std::function<void()> func;

public:
	repeat(unsigned long interval, std::function<void()> func) : lastTime(0), interval(interval), func(func) {}

	void operator()() {
		unsigned long currentTime = millis();
		if (currentTime - lastTime >= interval || lastTime == 0) {
			func();
			lastTime = currentTime;
		}
	}
};

} // namespace callback