#pragma once

#include "net.hpp"
#include "fs.hpp"
#include "logger.hpp"

namespace require
{
    static bool FS_INITIAL_CONNECT = false;

    void serial()
    {
        Serial.begin(115200);
        // Wait for serial to be ready
        while (!Serial)
        {
            pins::white();
            delay(50);
            pins::off();
            delay(1000);
        }
    }
}

namespace request
{
    static bool NET_AVAILABLE = false;

    void net()
    {
        if (NET_AVAILABLE && !net::connected())
        {
            net::tryConnect();
        }
    }

    void netInit()
    {
        NET_AVAILABLE = false;

        if (net::available())
        {
            // Read network credentials from the filesystem
            fs::Path ssidPath("/network.txt");
            auto info = ssidPath.readlines();
            String ssid, password;
            if (info.size() >= 2)
            {
                ssid = info[0];
                ssid.trim();
                password = info[1];
                password.trim();
            }

            if (info.size() < 2)
            {
                logger::error(
                    "Network credentials not found in " +
                    ssidPath.str() +
                    ".\n"
                    "Please create a file named 'network.txt' with SSID and password on separate lines.\n"
                    "Example content:\n"
                    "  MyNetworkSSID\n"
                    "  MyNetworkPassword");
                return;
            }

            net::init(ssid, password);
            NET_AVAILABLE = true;
        }
    }
}