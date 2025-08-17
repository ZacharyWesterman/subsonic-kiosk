#pragma once

#include "downloadQueue.hpp"
#include "uid.hpp"

#define REDIRECT_LIMIT 5

namespace util {

DownloadQueue::~DownloadQueue() {
	for (auto &download : downloads) {
		delete download; // Clean up allocated memory for each download
	}
}

int DownloadQueue::download(const fs::Path &file, const String &url) {
	int id = uid();
	// Make sure that the file is deleted before starting the download
	// otherwise we would just append garbage data onto some file
	if (file.exists()) {
		file.unlink();
	}
	auto dl = new Download{file, net::get(url), id};

	// Automatically follow redirects.
	int i = 0;
	while (i++ < REDIRECT_LIMIT && dl->request.redirected()) {
		auto location = dl->request.location();

		logger::info("Redirecting from " + url + " to " + location);

		if (location.isEmpty()) {
			break; // No location to redirect to, abort
		}

		delete dl;
		dl = new Download{file, net::get(location), id};
	}

	downloads.push_back(dl);
	return id;
}

bool DownloadQueue::finished(int id) const {
	for (const auto download : downloads) {
		if (download && download->id == id) {
			return download->request.done();
		}
	}
	return false;
}

void DownloadQueue::process() {
	for (auto download : downloads) {
		if (download && download->request.ready()) {
			download->file.write(download->request.data(), true);
		}
	}
}

void DownloadQueue::cleanup() {
	for (auto &download : downloads) {
		if (download && download->request.done()) {
			delete download;
			download = nullptr;
		}
	}

	auto condition = std::remove_if(downloads.begin(), downloads.end(), [](const Download *download) { return !download; });
	downloads.erase(condition, downloads.end());
}

} // namespace util
