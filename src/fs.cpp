#include "fs.hpp"
#include "logger.hpp"
#include "pins.hpp"
#include <vector>

#ifdef EMULATE
#include <cstdio>
#include <sys/stat.h>
#else
#include <Arduino_USBHostMbed5.h>
#endif

namespace fs {

#ifndef EMULATE
static USBHostMSD device;
static mbed::FATFileSystem filesystem("usb");
#endif

static bool initialized = false;

bool connect(int maxRetries) {
#ifndef EMULATE
	if (maxRetries < 0) {
		maxRetries = 10000; // Default to a large number if not specified
	}

	if (!device.connected()) {
		pins::yellow();

		initialized = false; // Reset initialization state
		filesystem.unmount();

		logger::info("Connecting USB device", false);
		// Attempt to connect to the USB mass storage device.
		for (int i = 0; i < maxRetries; ++i) {
			device.connect();
			if (device.connected()) {
				logger::raw("Done.\n");
				break;
			}
			logger::raw(".");
			delay(500);
		}

		if (!device.connected()) {
			logger::raw("Error.\n");
			logger::error("Failed to connect to USB device.");
			pins::off();
			return false;
		}
	}

	if (initialized) {
		return true; // Already initialized
	}

	// Mount the filesystem
	int error = filesystem.mount(&device);
	if (error) {
		logger::error("Failed to mount filesystem (Error code " + String(error) + ")");
		pins::off();
		return false;
	}

	pins::off();
#else
	struct stat st;
	if (stat("./usb", &st) != 0) {
		mkdir("./usb", 0755);
	}
#endif

	initialized = true;
	return true;
}

bool connected() {
#ifdef EMULATE
	return initialized;
#else
	return initialized && device.connected();
#endif
}

void disconnect() {
#ifndef EMULATE
	filesystem.unmount();
#endif
	initialized = false;
	logger::info("USB device disconnected.");
}

size_t size() {
	if (!connected()) {
		return 0;
	}

#ifndef EMULATE
	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return fsInfo.f_blocks * fsInfo.f_bsize;
#else
	// When emulating, just assume we have 1GB of disk space.
	return 1024 * 1024 * 1024;
#endif
}

size_t used() {
	if (!connected()) {
		return 0;
	}

#ifndef EMULATE
	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return (fsInfo.f_blocks * fsInfo.f_bsize - fsInfo.f_bfree * fsInfo.f_bsize);
#else
	return 0;
#endif
}

size_t free() {
	if (!connected()) {
		return 0;
	}

#ifndef EMULATE
	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return fsInfo.f_bfree * fsInfo.f_bsize;
#else
	// When emulating, just assume we have 1GB of disk space.
	return 1024 * 1024 * 1024;
#endif
}

String _path(const String &path) {
#ifndef EMULATE
	return "/usb" + path;
#else
	return "./usb" + path;
#endif
}

} // namespace fs
