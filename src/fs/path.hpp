#pragma once

#include "../fs.hpp"

namespace fs {

class Path; // Forward declaration

class IterDir {
	const Path &parent;
	DIR *dir;
	struct dirent *entry;

public:
	IterDir(const Path &parent, const String &path, bool end = false);

	~IterDir();

	bool operator!=(const IterDir &other) const;

	IterDir &operator++();

	const Path operator*() const;
};

class Path {
	String path;

public:
	Path(const String &path);

	bool exists() const;

	bool isDir() const;

	bool isFile() const;

	Path operator/(const String &subPath) const;

	Path &operator/=(const String &subPath);

	const String &str() const;

	const char *c_str() const;

	String name() const;

	String stem() const;

	String ext() const;

	String parent() const;

	String read() const;

	std::vector<String> readlines() const;

	bool write(const String &data, bool append = false) const;

	bool write(const std::vector<uint8_t> &data, bool append = false) const;

	bool mkdir(bool exist_ok = false) const;

	int size() const;

	FileStream stream() const;

	IterDir begin() const;

	IterDir end() const;
};

} // namespace fs