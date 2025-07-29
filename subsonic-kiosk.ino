
#define SERIAL_DEBUG

#include <WiFi.h>

#include "pins.hpp"
#include "net.hpp"
#include "fs.hpp"
#include "audio.hpp"

// audio::Player *player;
// int counter = 0;

void setup()
{
  pins::init();

#ifdef SERIAL_DEBUG
  Serial.begin(115200);
  // Wait for serial to be ready
  while (!Serial)
  {
    pins::white();
    delay(50);
    pins::off();
    delay(1000);
  }
#endif

  fs::connect();

  // Initialize the audio player with a specific file, if supported.
  // fs::Path filename("/spark.wav");
  // if (filename.isFile() && audio::supported(filename.ext()))
  // {
  //   player = new audio::Player("/spark.wav");
  //   player->play();
  // }

  // player.init();

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
    fs::connect();
  }

  /*
  if (player)
  {
    counter++;
    if (counter > 20000)
    {
      pins::off();
      counter = 0;
    }
    else if (counter > 10000)
    {
      pins::green();
    }

    player->output();

    if (counter % 10000 == 0)
    {
      logger::info("Elapsed: " + String(player->progress()) + "s / " + String(player->duration()) + "s.");
    }
  }
  */

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