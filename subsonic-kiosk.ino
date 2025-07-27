
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

  auto client = net::client("192.168.1.33", 8080);
  auto response = client.get("/airsonic/rest/ping.view?u=guest&p=guest&c=subsonic-kiosk&v=1.15.0&f=json");
  logger::info("Response status: " + String(response.status));
  if (response.ok())
  {
    auto json = response.json();
    logger::info("Response body: " + response.text());
    logger::info("Ping response: " + String(json["subsonic-response"]["status"].as<const char *>()));
    logger::info("Ping response: " + String(json["subsonic-response"]["version"].as<const char *>()));
  }

  delay(5000); // Wait before the next ping
}