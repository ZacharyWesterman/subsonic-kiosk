/// @file fileStream.hpp
#pragma once

#include "../logger.hpp"
#include <Arduino_USBHostMbed5.h>
#include <vector>

namespace fs {

/**
 * @brief A class to represent a file stream.
 * This class provides methods to read from a file in chunks and check the status of the stream.
 */
class FileStream {
	FILE *file;

public:
	/**
	 * @brief Constructor for the FileStream class.
	 * @param file A pointer to the file to be used for the stream. If nullptr, it will not be initialized.
	 */
	FileStream(FILE *file = nullptr);

	/**
	 * @brief Destructor for the FileStream class.
	 * This will close the file if it is open.
	 */
	~FileStream();

	/**
	 * @brief Check if the stream is in a good state.
	 * @return True if the stream is good, false otherwise.
	 */
	bool good() const;

	/**
	 * @brief Check if the stream is in a bad state.
	 * @return True if the stream is bad, false otherwise.
	 */
	bool bad() const;

	/**
	 * @brief Check if the stream is in a good state.
	 * @return True if the stream is good, false otherwise.
	 */
	inline operator bool() const {
		return good();
	}

	/**
	 * @brief Read data from the file stream.
	 * @tparam T The type of data to read from the file.
	 * @param chunkSize The size of the chunk to read.
	 * @return A vector containing the read data.
	 */
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

	/**
	 * @brief Seek to a specific position in the file.
	 * @param position The position to seek to in the file.
	 * @param flag The seek mode (SEEK_SET, SEEK_CUR, SEEK_END).
	 * @return True if the seek was successful, false otherwise.
	 */
	bool seek(size_t position, int flag = SEEK_SET);

	/**
	 * @brief Get the current position in the file.
	 * @return The current position in the file.
	 */
	size_t tell() const;

	/**
	 * @brief Get the size of the file.
	 * @return The size of the file in bytes.
	 */
	size_t size() const;
};

} // namespace fs
