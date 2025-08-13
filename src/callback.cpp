#include "callback.hpp"
#include <Arduino.h>

namespace callback {

repeat::repeat(unsigned long interval, std::function<void()> func) : lastTime(0), interval(interval), func(func) {}

void repeat::operator()() {
	unsigned long currentTime = millis();
	if (currentTime - lastTime >= interval || lastTime == 0) {
		func();
		lastTime = currentTime;
	}
}

} // namespace callback