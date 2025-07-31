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

    static void netConfigError()
    {
        logger::error(
            "Failed to read network configuration file."
            "\nPlease create a file named 'network.json' with the following structure:\n"
            "Example content:\n"
            "{\n"
            "  \"ssid\": \"MyNetworkSSID\",\n"
            "  \"password\": \"MyNetworkPassword\"\n"
            "}");
    }

    void netInit()
    {
        NET_AVAILABLE = false;

        if (net::available())
        {
            // Read network credentials from the filesystem
            fs::Path ssidPath("/network.json");
            if (!ssidPath.isFile())
            {
                netConfigError();
                return;
            }

            // Sanity check in case the file is too large
            if (ssidPath.size() > 5 * 1024)
            {
                netConfigError();
                return;
            }

            // Deserialize the JSON file
            JsonDocument config;
            auto error = deserializeJson(config, ssidPath.read().c_str());
            if (error)
            {
                netConfigError();
                return;
            }

            // Make sure the JSON has the required fields
            if (!config.is<JsonObject>() ||
                !config["ssid"].is<JsonString>() ||
                !config["password"].is<JsonString>())
            {
                netConfigError();
                return;
            }

            net::init(config["ssid"].as<JsonString>().c_str(), config["password"].as<JsonString>().c_str());
            NET_AVAILABLE = true;
        }
    }
}