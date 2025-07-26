#pragma once

#if !__has_include("secret/network.hpp")

static_assert(false, "Network configuration file not found.\nPlease create 'secret/network.hpp' with the following definitions:\n#define NETWORK_SSID \"your_ssid\"\n#define NETWORK_PASS \"your_password\"");
#define NETWORK_SSID "your_ssid"
#define NETWORK_PASS "your_password"

#else

#include "secret/network.hpp"

#ifndef NETWORK_SSID
#error "NETWORK_SSID must be defined"
#endif

#ifndef NETWORK_PASS
#error "NETWORK_PASS must be defined"
#endif

#endif

#include <WiFi.h>

#include "logger.hpp"

namespace net
{
    void connect(int maxRetries = -1)
    {
        if (WiFi.status() == WL_NO_MODULE)
        {
            logger::fatal("WiFi module not installed!");
        }

        logger::info("Initializing network...");

        int status = WiFi.begin(NETWORK_SSID, NETWORK_PASS);

        int retries = 0;
        while (
            status != WL_CONNECTED && (retries <= maxRetries ||
                                       maxRetries < 0))
        {
            logger::info("Attempting to connect to WPA SSID: " + String(NETWORK_SSID));

            status = WiFi.begin(NETWORK_SSID, NETWORK_PASS);
            retries++;

            if (status == WL_CONNECTED)
            {
                continue;
            }

            // Wait a while for connection
            logger::info("Waiting for connection...", false);
            for (int i = 5; i > 0; --i)
            {
                delay(1000);
                status = WiFi.status();
                if (status == WL_CONNECTED)
                {
                    break;
                }
                logger::raw(".");
            }
            logger::raw("\n");
            logger::info(String(" Retrying (") + retries + "/" + maxRetries + ")...");
        }

        if (status != WL_CONNECTED)
        {
            logger::warn("Failed to connect to WiFi network.");
            return;
        }

        logger::info("Successfully connected to WiFi network.");

        // Print network info
        logger::info("SSID: " + String(WiFi.SSID()));

        byte bssid[6];
        WiFi.BSSID(bssid);
        logger::info(
            "BSSID: " +
            String(bssid[0], HEX) + ":" +
            String(bssid[1], HEX) + ":" +
            String(bssid[2], HEX) + ":" +
            String(bssid[3], HEX) + ":" +
            String(bssid[4], HEX) + ":" +
            String(bssid[5], HEX));

        long rssi = WiFi.RSSI();
        logger::info("Signal strength (RSSI): " + String(rssi));

        byte encryption = WiFi.encryptionType();
        logger::info("Encryption Type: " + String(encryption, HEX));

        IPAddress ip = WiFi.localIP();
        logger::info("IP Address: " + ip.toString());

        // print your MAC address:
        byte mac[6];
        WiFi.macAddress(mac);
        logger::info(
            "MAC address: " +
            String(mac[0], HEX) + ":" +
            String(mac[1], HEX) + ":" +
            String(mac[2], HEX) + ":" +
            String(mac[3], HEX) + ":" +
            String(mac[4], HEX) + ":" +
            String(mac[5], HEX));

        Serial.println();
    }

    void disconnect()
    {
        logger::info("Disconnecting from WiFi... ", false);
        WiFi.disconnect();
        logger::raw("Done.\n");
    }

    bool connected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    int ping(const char *host, int timeout = 5000)
    {
        if (!connected())
        {
            return -1; // Not connected
        }

        return WiFi.ping(host, timeout);
    }
}
