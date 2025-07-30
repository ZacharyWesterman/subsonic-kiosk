#pragma once

#include <WiFi.h>

#include "pins.hpp"
#include "logger.hpp"
#include "net/netClient.hpp"

namespace net
{
    static String NETWORK_SSID;
    static String NETWORK_PASS;
    static unsigned long LAST_CONNECT_TIME = 0;
    static bool CONNECTING = false;

    bool available()
    {
        return WiFi.status() != WL_NO_MODULE;
    }

    void init(const String &ssid, const String &password)
    {
        NETWORK_SSID = ssid;
        NETWORK_PASS = password;

        if (!available())
        {
            logger::fatal("WiFi module not installed!");
        }
    }

    bool connected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    void tryConnect()
    {
        if (!available())
        {
            logger::fatal("WiFi module not installed!");
        }

        if (connected())
        {
            logger::info("Connected to WiFi network: " + NETWORK_SSID);
            pins::off();
            CONNECTING = false;
            return;
        }

        if (CONNECTING)
        {
            unsigned long now = millis();
            if (abs(now - LAST_CONNECT_TIME) < 5000)
            {
                return; // Avoid spamming connection attempts
            }
        }

        pins::blue();
        logger::info("Connecting to WiFi network: " + NETWORK_SSID);
        WiFi.begin(NETWORK_SSID.c_str(), NETWORK_PASS.c_str());

        CONNECTING = true;
        LAST_CONNECT_TIME = millis();

        if (connected())
        {
            logger::info("Connected to WiFi network: " + NETWORK_SSID);
            pins::off();
            CONNECTING = false;
            return;
        }
    }

    /*
    void connect(String ssid, String password, int maxRetries = -1)
    {

        logger::info("Initializing network...");
        pins::blue();

        int status = WiFi.begin(NETWORK_SSID, NETWORK_PASS);
        pins::white();

        int retries = 0;
        while (
            status != WL_CONNECTED && (retries <= maxRetries ||
                                       maxRetries < 0))
        {
            logger::info("Attempting to connect to WPA SSID: " + ssid);

            status = WiFi.begin(ssid, password);
            retries++;
            pins::blue();

            if (status == WL_CONNECTED)
            {
                continue;
            }

            // Wait a while for connection
            logger::info("Waiting for connection...", false);
            for (int i = 5; i > 0; --i)
            {
                pins::white();
                delay(500);
                pins::blue();
                delay(500);
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
            pins::off();
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
        pins::off();
    }
    */

    void disconnect()
    {
        logger::info("Disconnecting from WiFi... ", false);
        WiFi.disconnect();
        logger::raw("Done.\n");
    }

    int ping(const char *host, int timeout = 5000)
    {
        if (!connected())
        {
            return -1; // Not connected
        }

        return WiFi.ping(host, timeout);
    }

    NetClient client(const String &host, int port = 80)
    {
        return NetClient(host, port);
    }
}
