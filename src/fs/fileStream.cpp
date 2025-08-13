#include "fileStream.hpp"
#include "../logger.hpp"
#include <vector>

namespace fs {

FileStream::FileStream(FILE *file) : file(file) {}

FileStream::~FileStream() {
	if (file) {
		fclose(file);
	}
}

bool FileStream::good() const {
	return file != nullptr && !ferror(file);
}

bool FileStream::bad() const {
	return file == nullptr || ferror(file);
}

bool FileStream::seek(size_t position, int flag) {
	if (!file) {
		logger::error("FileStream is not initialized.");
		return false;
	}

	if (fseek(file, position, flag) != 0) {
		logger::error("Failed to seek in file.");
		return false;
	}
	return true;
}

size_t FileStream::tell() const {
	return ftell(file);
}

size_t FileStream::size() const {
	if (!file) {
		logger::error("FileStream is not initialized.");
		return 0;
	}

	auto currentPos = ftell(file);
	fseek(file, 0, SEEK_END);
	auto fileSize = ftell(file);
	fseek(file, currentPos, SEEK_SET);
	return fileSize;
}

} // namespace fs