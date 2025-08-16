#pragma once

#include "downloadQueue.hpp"
#include "uid.hpp"

namespace util {

DownloadQueue::~DownloadQueue() {
	for (auto &download : downloads) {
		delete download; // Clean up allocated memory for each download
	}
}

int DownloadQueue::download(const fs::Path &file, const String &hostname, const String &url) {
	int id = uid();
	// Make sure that the file is deleted before starting the download
	// otherwise we would just append garbage data onto some file
	file.unlink();
	downloads.push_back(new Download{file, net::client(hostname).get(url), id});
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
