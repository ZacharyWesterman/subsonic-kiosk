#pragma once

#include "../logger.hpp"
#include <Arduino_USBHostMbed5.h>
#include <vector>

namespace fs {

class FileStream {
	FILE *file;

public:
	FileStream(FILE *file = nullptr);

	~FileStream();

	bool good() const;

	bool bad() const;

	inline operator bool() const {
		return good();
	}

	template <typename T>
	std::vector<T> read(int chunkSize = 1024) {
		if (!file) {
			logger::error("FileStream is not initialized.");
			return {};
		}

		std::vector<T> buffer(chunkSize);
		size_t dataRead = fread(buffer.data(), sizeof(T), chunkSize, file);
		if (dataRead == 0 && ferror(file)) {
			logger::error("Error reading from file.");
			return {};
		}

		buffer.resize(dataRead);
		return buffer;
	}

	bool seek(size_t position, int flag = SEEK_SET);

	size_t tell() const;

	size_t size() const;
};

} // namespace fs