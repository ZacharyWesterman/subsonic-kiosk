#pragma once

#include "../fs.hpp"

namespace fs {

class Path; // Forward declaration

/**
 * @brief An iterator class for iterating over directories.
 * This class allows iteration over the contents of a directory,
 * and is provided to work with the range-based for loop syntax.
 */
class IterDir {
	const Path &parent;
	DIR *dir;
	struct dirent *entry;

public:
	/**
	 * @brief Constructor for the IterDir class.
	 * @param parent The parent Path object to iterate over.
	 * @param path The path of the directory to iterate over.
	 * @param end If true, initializes the iterator to the end state.
	 */
	IterDir(const Path &parent, const String &path, bool end = false);

	/**
	 * @brief Destructor for the IterDir class.
	 * This will close the directory if it is open.
	 */
	~IterDir();

	/**
	 * @brief Check if the iterator is at the end.
	 * @return True if the iterator is at the end, false otherwise.
	 */
	bool operator!=(const IterDir &other) const;

	/**
	 * @brief Increment the iterator to the next entry.
	 * @return A reference to the current iterator.
	 */
	IterDir &operator++();

	/**
	 * @brief Dereference the iterator to get the current Path object.
	 * @return The current Path object.
	 */
	const Path operator*() const;
};

/**
 * @brief A class to represent a file path.
 * This class provides methods to manipulate and query file paths.
 */
class Path {
	String path;

public:
	/**
	 * @brief Constructor for the Path class.
	 * @param path The file path to be represented by this Path object.
	 */
	Path(const String &path);

	/**
	 * @brief Check if the path exists.
	 * @return True if the path exists, false otherwise.
	 */
	bool exists() const;

	/**
	 * @brief Check if the path exists and is a directory.
	 * @return True if the path is a directory, false otherwise.
	 */
	bool isDir() const;

	/**
	 * @brief Check if the path exists and is a file.
	 * @return True if the path is a file, false otherwise.
	 */
	bool isFile() const;

	/**
	 * @brief Append a sub-path to the current path.
	 * @param subPath The sub-path to append.
	 * @return A new Path object representing the combined path.
	 */
	Path operator/(const String &subPath) const;

	/**
	 * @brief Append a sub-path to the current path in place.
	 * @param subPath The sub-path to append.
	 * @return A reference to the current Path object.
	 */
	Path &operator/=(const String &subPath);

	/**
	 * @brief Get the string representation of the path.
	 * @return The string representation of the path.
	 */
	const String &str() const;

	/**
	 * @brief Get the C-style string representation of the path.
	 * @return The C-style string representation of the path.
	 */
	const char *c_str() const;

	/**
	 * @brief Get the base name of the path.
	 * @return The base name of the path (the last component).
	 */
	String name() const;

	/**
	 * @brief Get the stem of the path (the base name without the extension).
	 * @return The stem of the path.
	 */
	String stem() const;

	/**
	 * @brief Get the file extension of the path.
	 * @return The file extension of the path.
	 */
	String ext() const;

	/**
	 * @brief Get the parent directory of the path.
	 * @return A new Path object representing the parent directory.
	 */
	String parent() const;

	/**
	 * @brief Read the contents of the file at this path.
	 * @return The contents of the file as a String.
	 *
	 * @note If the path is not a file or does not exist, an empty string is returned.
	 * @warning This function will block until the entire file is read.
	 */
	String read() const;

	/**
	 * @brief Read the contents of the file at this path as lines.
	 * @return A vector of strings, each representing a line in the file.
	 *
	 * @note If the path is not a file or does not exist, an empty vector is returned.
	 * @warning This function will block until the entire file is read.
	 */
	std::vector<String> readlines() const;

	/**
	 * @brief Write data to the file at this path.
	 * @param data The data to write to the file.
	 * @param append If true, append to the file; if false, overwrite the file.
	 * @return True if the write was successful, false otherwise.
	 */
	bool write(const String &data, bool append = false) const;

	/**
	 * @brief Write binary data to the file at this path.
	 * @param data The binary data to write to the file.
	 * @param append If true, append to the file; if false, overwrite the file.
	 * @return True if the write was successful, false otherwise.
	 */
	bool write(const std::vector<uint8_t> &data, bool append = false) const;

	/**
	 * @brief Create a directory at this path.
	 * @param exist_ok If true, do not return false if the directory already exists.
	 * @return True if the directory was created successfully, false otherwise.
	 */
	bool mkdir(bool exist_ok = false) const;

	/**
	 * @brief Get the size of the file at this path.
	 * @return The size of the file in bytes, or -1 if the path is not a file or does not exist.
	 */
	int size() const;

	/**
	 * @brief Remove the file or directory at this path.
	 * @param recurse If true, remove directories and their contents recursively.
	 * @return True if the removal was successful, false otherwise.
	 * @note If the path is a non-empty directory and recurse is false, the removal will fail.
	 */
	bool unlink(bool recurse = false) const;

	/**
	 * @brief Get a file stream for the file at this path.
	 * @return A FileStream object for the file.
	 * @note If the path is not a file or does not exist, the FileStream will be empty.
	 */
	FileStream stream() const;

	/**
	 * @brief Get an iterator for the directory contents.
	 * @return An IterDir object for iterating over the directory contents.
	 * @note If the path is not a directory, the iterator will be empty.
	 */
	IterDir begin() const;

	/**
	 * @brief Get an end iterator for the directory contents.
	 * @return An IterDir object representing the end of the directory contents.
	 */
	IterDir end() const;
};

} // namespace fs
