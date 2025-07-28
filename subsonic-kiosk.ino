
#define DEBUG

#include <WiFi.h>

#include "pins.hpp"
#include "net.hpp"
#include "fs.hpp"

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial)
    ; // Wait until serial is up and working.
#endif
  pins::init();

  fs::connect();
  // net::connect(3);
}

void loop()
{
  // if (!net::connected())
  // {
  //   logger::warn("Network disconnected! Attempting to reconnect...");
  //   net::connect(0);
  //   return;
  // }

  if (!fs::connected())
  {
    logger::warn("USB device disconnected! Attempting to reconnect...");
    fs::connect(0);
    return;
  }

  /*
  logger::info("Directory listing for /:");
  for (auto &path : fs::Path("/"))
  {
    if (path.isDir())
    {
      logger::info("Directory: " + path.str());
    }
    else if (path.isFile())
    {
      logger::info("File: " + path.str());
      logger::info("File extension: " + path.ext());
      logger::info("File size: " + String(path.size()) + " bytes");
      logger::info("File content: " + String((const char *)path.stream().read().data()));
    }
    else
    {
      logger::warn("Unknown type: " + path.str());
    }
  }
  */

  /*
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
  */

  delay(5000); // Wait before the next ping
}