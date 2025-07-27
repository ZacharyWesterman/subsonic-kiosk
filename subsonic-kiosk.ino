
#define DEBUG

#include <WiFi.h>

#include "pins.hpp"
#include "net.hpp"

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
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

  auto client = net::client("www.google.com");
  auto response = client.get("/");

  logger::info("Response status: " + String(response.status));
  if (response.ok())
  {
    logger::info("Response body: " + response.body());
  }

  delay(5000); // Wait before the next ping
}