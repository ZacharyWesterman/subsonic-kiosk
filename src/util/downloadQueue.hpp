/// @file downloadQueue.hpp
#pragma once

#include "../fs/path.hpp"
#include "../net.hpp"

namespace util {

struct Download {
	fs::Path file;
	net::Request request;
	int id;
};

class DownloadQueue {
	std::vector<Download *> downloads;

public:
	~DownloadQueue();

	int download(const fs::Path &file, const String &hostname, const String &url);

	bool finished(int id) const;

	void process();

	void cleanup();
};

} // namespace util
