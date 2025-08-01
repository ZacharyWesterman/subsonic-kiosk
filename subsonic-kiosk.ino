
#define SERIAL_DEBUG

#include "pins.hpp"
#include "net.hpp"
#include "fs.hpp"
#include "audio.hpp"

#include "require.hpp"

audio::Player *player;
// int counter = 0;
bool NET_AVAILABLE = false;

void setup()
{
  pins::init();

#ifdef SERIAL_DEBUG
  require::serial(); // Initialize serial communication
#endif

  fs::connect();      // Ensure filesystem is connected
  request::netInit(); // Try to connect to the network, if available

  // Initialize the audio player with a specific file, if supported.
  // fs::Path filename("/spark.wav");
  // if (filename.isFile() && audio::supported(filename.ext()))
  // {
  //   player = new audio::Player(filename.str());
  //   player->play();
  // }

  // player.init();

  // net::connect(3);
}

void loop()
{
  if (!fs::connected())
  {
    logger::warn("USB device disconnected! Attempting to reconnect...");
    fs::connect();

    request::netInit(); // Reinitialize network if USB is reconnected
    return;
  }

  request::net(); // Try to keep the network connected

  // request::net(); // Try to connect to the network, if available

  // logger::info("Busy waiting... " + String(millis() / 1000) + "s");

  // if (!net::connected())
  // {
  //   logger::warn("Network disconnected! Attempting to reconnect...");
  //   net::connect(0);
  //   return;
  // }

  // if (player && player->good())
  // {
  //   /*
  //   counter++;
  //   if (counter > 20000)
  //   {
  //     pins::off();
  //     counter = 0;
  //   }
  //   else if (counter > 10000)
  //   {
  //     pins::green();
  //   }
  //   */

  //   if (player->finished())
  //   {
  //     logger::info("Playback finished.");
  //     delete player;
  //   }
  //   else
  //   {
  //     player->output();
  //   }

  //   /*
  //   if (counter % 10000 == 0)
  //   {
  //     logger::info("Elapsed: " + String(player->progress()) + "s / " + String(player->duration()) + "s.");
  //   }
  //   */
  // }

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

  // delay(5000); // Wait before the next ping
}