/// @file downloadQueue.hpp
#pragma once

#include "../fs/path.hpp"
#include "../net.hpp"

namespace util {

/**
 * @brief A struct representing a download.
 */
struct Download {
	/// The destination file path.
	fs::Path file;
	/// The network request associated with the download.
	net::Request request;
	/// The unique identifier for the download.
	int id;
};

/**
 * @brief An iterator for the download queue.
 */
class DownloadQueueIter {
	std::vector<Download *>::iterator it;

public:
	/**
	 * @brief Construct a new DownloadQueueIter.
	 * @param it The iterator to wrap.
	 */
	DownloadQueueIter(std::vector<Download *>::iterator it) : it(it) {}

	/**
	 * @brief Dereference operator.
	 * @return A const reference to the download.
	 */
	const Download &operator*() const {
		return **it;
	}

	/**
	 * @brief Pre-increment operator.
	 * @return A reference to the updated iterator.
	 */
	DownloadQueueIter &operator++() {
		++it;
		return *this;
	}

	/**
	 * @brief Comparison operator.
	 * @return True if the iterators are not equal, false otherwise.
	 */
	bool operator!=(const DownloadQueueIter &other) const {
		return it != other.it;
	}
};

/**
 * @brief A class for managing a queue of downloads.
 */
class DownloadQueue {
	std::vector<Download *> downloads;

public:
	/// Destructor.
	~DownloadQueue();

	/**
	 * @brief Add a download to the queue.
	 * @param file The destination file path.
	 * @param url The URL for the download.
	 * @return The unique identifier for the download.
	 */
	int download(const fs::Path &file, const String &url);

	/**
	 * @brief Check if a download is finished.
	 * @param id The unique identifier for the download.
	 * @return True if the download is finished, false otherwise.
	 */
	bool finished(int id) const;

	/**
	 * @brief Process the download queue, appending an available data to the relevant files.
	 * @note This does not remove finished downloads from the queue.
	 */
	void process();

	/**
	 * @brief Clean up finished downloads.
	 */
	void cleanup();

	/// Get an iterator to the beginning of the download queue.
	DownloadQueueIter begin() {
		return DownloadQueueIter(downloads.begin());
	}

	/// Get an iterator to the end of the download queue.
	DownloadQueueIter end() {
		return DownloadQueueIter(downloads.end());
	}
};

} // namespace util
