#pragma once

#include <WiFi.h>
#include "logger.hpp"

namespace net
{
    class Request
    {
        WiFiClient &client;
        bool finished = false;
        String responseBody;

    public:
        int status;

        Request(WiFiClient &client) : client(client)
        {
            status = 400; // Default to bad request

            // Read the response status line
            String statusLine = readln();
            int i1 = statusLine.indexOf(' ');
            if (i1 != -1)
            {
                int i2 = statusLine.indexOf(' ', i1 + 1);
                if (i2 != -1)
                {
                    status = statusLine.substring(i1 + 1, i2).toInt();
                }
            }
        }

        inline operator bool() const
        {
            return ok();
        }

        bool done() const
        {
            return finished;
        }

        bool ok() const
        {
            return status >= 200 && status < 300;
        }

        String readln()
        {
            String line;
            if (finished)
            {
                return line; // Return empty line if already finished
            }

            while (client.connected())
            {
                int bytes = client.available();
                bool lineComplete = false;

                if (bytes == 0)
                {
                    delay(10); // Wait for data to be available
                    continue;
                }

                line.reserve(line.length() + bytes);
                for (int i = 0; i < bytes; ++i)
                {
                    char c = client.read();
                    if (c == '\n')
                    {
                        lineComplete = true; // End of line
                        break;
                    }
                    if (c != '\r') // Ignore carriage return
                    {
                        line += c;
                    }
                }

                if (lineComplete)
                {
                    break;
                }
            }

            if (!client.connected())
            {
                finished = true; // Mark as finished if connection is lost
            }

            return line;
        }

        String body()
        {
            if (finished)
            {
                return responseBody;
            }

            bool foundBody = false;
            while (client.connected())
            {
                if (readln().length() == 0)
                {
                    foundBody = true; // Empty line indicates end of headers
                    break;
                }
            }

            if (foundBody)
            {
                readln();                   // Skip line before the actual body
                String lastLine = readln(); // Read the first line of the body

                while (client.connected())
                {
                    String line = readln();
                    if (line.length() == 0)
                    {
                        break; // End of body
                    }
                    responseBody += lastLine + "\n"; // Append last line to response body
                    lastLine = line;                 // Update last line for the next iteration
                }
            }

            finished = true; // Mark as finished after reading the body
            return responseBody;
        }
    };

    class NetClient
    {
        WiFiClient client;
        String host;
        int port;
        bool connected = false;

    public:
        NetClient(const String &host, int port) : host(host), port(port)
        {
            connected = client.connect(host.c_str(), port);
        }

        ~NetClient()
        {
            if (connected)
            {
                client.stop();
            }
        }

        Request get(const String &path)
        {
            if (!connected)
            {
                connected = client.connect(host.c_str(), port);
            }

            if (!connected)
            {
                return Request(client);
            }

            client.println("GET " + path + " HTTP/1.1");
            client.println("Host: " + host);
            client.println("Connection: close");
            client.println();

            Request response(client);
            return response;
        }
    };
}