#pragma once

#include <Arduino.h>

namespace logger {

void fatal(const String &message);

void error(const String &message);

void warn(const String &message);

void info(const String &message, bool lineBreak = true);

void raw(const String &message);

void test();

} // namespace logger
