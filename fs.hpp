#pragma once

#include <Arduino_USBHostMbed5.h>
#include <vector>

#include "pins.hpp"
#include "logger.hpp"

namespace fs
{
    static USBHostMSD device;
    static mbed::FATFileSystem filesystem("usb");
    static bool initialized = false;

    bool connect(int maxRetries = -1)
    {
        if (maxRetries < 0)
        {
            maxRetries = 10000; // Default to a large number if not specified
        }

        if (!device.connected())
        {
            pins::yellow();

            initialized = false; // Reset initialization state
            filesystem.unmount();

            logger::info("Connecting USB device... ", false);
            // Attempt to connect to the USB mass storage device.
            for (int i = 0; i < maxRetries; ++i)
            {
                if (device.connect())
                {
                    logger::raw("Done.\n");
                    break;
                }
                logger::raw(".");
                delay(500);
            }

            if (!device.connected())
            {
                logger::raw("Error.\n");
                logger::error("Failed to connect to USB device.");
                pins::off();
                return false;
            }
        }

        if (initialized)
        {
            return true; // Already initialized
        }

        // Mount the filesystem
        int error = filesystem.mount(&device);
        if (error)
        {
            logger::error("Failed to mount filesystem (Error code " + String(error) + ")");
            pins::off();
            return false;
        }

        initialized = true;
        return true;
    }

    bool connected()
    {
        return initialized && device.connected();
    }

    void disconnect()
    {
        filesystem.unmount();
        initialized = false;
        logger::info("USB device disconnected.");
    }

    static String _path(const String &path)
    {
        return "/usb" + path;
    }
}

#include "fileStream.hpp"
#include "path.hpp"
