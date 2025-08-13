#include "pins.hpp"
#include <Arduino.h>

#define LED_RED 86
#define LED_GREEN 87
#define LED_BLUE 88

namespace pins {

void red() {
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_BLUE, HIGH);
}

void green() {
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, HIGH);
}

void blue() {
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_BLUE, LOW);
}

void cyan() {
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
}

void magenta() {
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_BLUE, LOW);
}

void yellow() {
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, HIGH);
}

void white() {
	digitalWrite(LED_RED, LOW);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
}

void off() {
	digitalWrite(LED_RED, HIGH);
	digitalWrite(LED_GREEN, HIGH);
	digitalWrite(LED_BLUE, HIGH);
}

inline void black() {
	off();
}

void init() {
	pinMode(LED_RED, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	off();
}

} // namespace pins
