#include "logger.hpp"
#include "pins.hpp"
#include <Arduino.h>

namespace logger {

void fatal(const String &message) {
#ifdef DEBUG
	Serial.println("FATAL ERROR: " + message);
#endif

	// Blink red LED to indicate fatal error
	while (true) {
		pins::red();
		delay(500);
		pins::off();
		delay(500);
	}
}

void error(const String &message) {
#ifdef DEBUG
	Serial.println("ERROR: " + message);
#endif

	// Blink red LED to indicate error
	pins::red();
	delay(500);
	pins::off();
}

void warn(const String &message) {
#ifdef DEBUG
	Serial.println("WARNING: " + message);
#endif

	// Blink yellow LED to indicate warning
	pins::yellow();
	delay(100);
	pins::off();
	delay(100);
	pins::yellow();
	delay(100);
	pins::off();
}

void info(const String &message, bool lineBreak) {
#ifdef DEBUG
	Serial.print("INFO: " + message);
	if (lineBreak) {
		Serial.println();
	}
#endif
}

void raw(const String &message) {
#ifdef DEBUG
	Serial.print(message);
#endif
}

#ifdef DEBUG
void test() {
	Serial.println("Logger test function called.");

	// Cycle through colors to indicate logger is working
	pins::red();
	delay(500);
	pins::yellow();
	delay(500);
	pins::green();
	delay(500);
	pins::cyan();
	delay(500);
	pins::blue();
	delay(500);
	pins::magenta();
	delay(500);
	pins::white();
	delay(500);
	pins::off();
	delay(1500);
}
#endif

} // namespace logger
