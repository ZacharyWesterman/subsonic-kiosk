
#define DEBUG

#include <WiFi.h>

#include "pins.hpp"
#include "net.hpp"

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial)
    ; // Wait until serial is up and working.
#endif

  pins::init();
  net::connect(3);
}

void loop()
{
  if (!net::connected())
  {
    logger::warn("Network disconnected! Attempting to reconnect...");
    net::connect(0);
    return;
  }

  delay(5000); // Wait before the next ping
}