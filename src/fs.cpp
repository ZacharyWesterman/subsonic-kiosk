#include "fs.hpp"
#include "logger.hpp"
#include "pins.hpp"
#include <Arduino_USBHostMbed5.h>
#include <vector>

namespace fs {

static USBHostMSD device;
static mbed::FATFileSystem filesystem("usb");
static bool initialized = false;

bool connect(int maxRetries) {
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
	initialized = true;
	return true;
}

bool connected() {
	return initialized && device.connected();
}

void disconnect() {
	filesystem.unmount();
	initialized = false;
	logger::info("USB device disconnected.");
}

size_t size() {
	if (!connected()) {
		return 0;
	}

	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return fsInfo.f_blocks * fsInfo.f_bsize;
}

size_t used() {
	if (!connected()) {
		return 0;
	}

	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return (fsInfo.f_blocks * fsInfo.f_bsize - fsInfo.f_bfree * fsInfo.f_bsize);
}

size_t free() {
	if (!connected()) {
		return 0;
	}

	struct statvfs fsInfo;
	if (statvfs("/usb", &fsInfo) != 0) {
		logger::error("Failed to get filesystem info.");
		return 0;
	}
	return fsInfo.f_bfree * fsInfo.f_bsize;
}

String _path(const String &path) {
	return "/usb" + path;
}

} // namespace fs
