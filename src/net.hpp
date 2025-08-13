#pragma once

#include "net/netClient.hpp"

namespace net {

extern String NETWORK_SSID;
extern String NETWORK_PASS;

bool available();

void init(const String &ssid, const String &password);

bool connected();

void tryConnect();

void disconnect();

int ping(const char *host, int timeout = 5000);

NetClient client(const String &host, int port = 80);

} // namespace net
