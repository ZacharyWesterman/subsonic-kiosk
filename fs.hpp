#pragma once

#include <Arduino_USBHostMbed5.h>
#include <vector>
#include <array>

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

    class FileStream
    {
        FILE *file;

    public:
        FileStream(FILE *file = nullptr) : file(file) {}

        ~FileStream()
        {
            if (file)
            {
                fclose(file);
            }
        }

        std::array<uint8_t, 256> read()
        {
            if (!file)
            {
                logger::error("FileStream is not initialized.");
                return {};
            }

            std::array<uint8_t, 256> buffer = {};
            size_t bytesRead = fread(buffer.data(), 1, buffer.size(), file);
            if (bytesRead == 0 && ferror(file))
            {
                logger::error("Error reading from file.");
                return buffer;
            }

            return buffer;
        }
    };

    class Path; // Forward declaration

    class IterDir
    {
        const Path &parent;
        DIR *dir;
        struct dirent *entry;

    public:
        IterDir(const Path &parent, const String &path, bool end = false)
            : parent(parent)
        {
            dir = nullptr;
            entry = nullptr;

            if (!end && connected())
            {
                dir = opendir(_path(path).c_str());
                if (dir)
                {
                    entry = readdir(dir);
                }
            }
        }

        ~IterDir()
        {
            if (dir && connected())
            {
                closedir(dir);
            }
        }

        bool operator!=(const IterDir &other) const
        {
            return entry != nullptr;
        }

        IterDir &operator++()
        {
            if (dir)
            {
                entry = readdir(dir);
            }
            return *this;
        }

        const Path operator*() const;
    };

    class Path
    {
        String path;

    public:
        Path(const String &path)
        {
            if (path.endsWith("/"))
            {
                this->path = path.substring(0, path.length() - 1); // Remove trailing slash
            }
            else
            {
                this->path = path;
            }
        }

        bool exists() const
        {
            if (!fs::connected())
            {
                return false;
            }
            struct stat st;
            return stat(_path(path).c_str(), &st) == 0;
        }

        bool isDir() const
        {
            if (!fs::connected())
            {
                return false;
            }
            struct stat st;
            if (stat(_path(path).c_str(), &st) != 0)
            {
                return false;
            }
            return S_ISDIR(st.st_mode);
        }

        bool isFile() const
        {
            if (!fs::connected())
            {
                return false;
            }
            struct stat st;
            if (stat(_path(path).c_str(), &st) != 0)
            {
                return false;
            }
            return S_ISREG(st.st_mode);
        }

        Path operator/(const String &subPath) const
        {
            return Path(path + "/" + subPath);
        }

        Path &operator/=(const String &subPath)
        {
            path += "/" + subPath;
            return *this;
        }

        const String &str() const
        {
            return path;
        }

        const char *c_str() const
        {
            return path.c_str();
        }

        String name() const
        {
            int lastSlash = path.lastIndexOf('/');
            if (lastSlash == -1)
            {
                return path;
            }
            return path.substring(lastSlash + 1);
        }

        String stem() const
        {
            String n = name();
            int dotIndex = n.lastIndexOf('.');
            if (dotIndex == -1)
            {
                return n;
            }
            return n.substring(0, dotIndex);
        }

        String ext() const
        {
            String n = name();
            int dotIndex = n.lastIndexOf('.');
            if (dotIndex == -1)
            {
                return ""; // No extension
            }
            return n.substring(dotIndex + 1);
        }

        String parent() const
        {
            int lastSlash = path.lastIndexOf('/');
            if (lastSlash == -1)
            {
                return ""; // No parent directory
            }
            return path.substring(0, lastSlash);
        }

        String read() const
        {
            if (!connected())
            {
                return "";
            }

            auto file = fopen(_path(path).c_str(), "r");
            if (!file)
            {
                logger::error("Failed to open file for reading: " + path);
                return "";
            }

            String content;
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file))
            {
                content += buffer;
            }

            fclose(file);
            return content;
        }

        bool write(const String &data) const
        {
            if (!connected())
            {
                return false;
            }

            auto file = fopen(_path(path).c_str(), "w");
            if (!file)
            {
                logger::error("Failed to open file for writing: " + path);
                return false;
            }

            size_t written = fwrite(data.c_str(), 1, data.length(), file);
            fclose(file);

            if (written != data.length())
            {
                logger::error("Failed to write all data to file: " + path);
                return false;
            }

            return true;
        }

        bool write(const std::vector<uint8_t> &data, bool append = false) const
        {
            if (!connected())
            {
                return false;
            }

            auto file = fopen(_path(path).c_str(), append ? "ab" : "wb");
            if (!file)
            {
                logger::error("Failed to open file for writing: " + path);
                return false;
            }

            size_t written = fwrite(data.data(), 1, data.size(), file);
            fclose(file);

            if (written != data.size())
            {
                logger::error("Failed to write all data to file: " + path);
                return false;
            }

            return true;
        }

        int size() const
        {
            if (!connected())
            {
                return -1;
            }

            struct stat st;
            if (stat(_path(path).c_str(), &st) != 0)
            {
                logger::error("Failed to get file size: " + path);
                return -1;
            }
            return st.st_size;
        }

        FileStream stream() const
        {
            if (!connected())
            {
                return FileStream();
            }

            auto file = fopen(_path(path).c_str(), "r");
            if (!file)
            {
                logger::error("Failed to open file for streaming: " + path);
                return FileStream();
            }

            return FileStream(file);
        }

        IterDir begin() const
        {
            return IterDir(*this, path);
        }

        IterDir end() const
        {
            return IterDir(*this, path, true);
        }
    };

    const Path IterDir::operator*() const
    {
        if (entry)
        {
            return parent / String(entry->d_name);
        }
        return Path(""); // Return an empty path if no entry
    }
}