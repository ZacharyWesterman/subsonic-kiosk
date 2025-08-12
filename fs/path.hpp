#pragma once

namespace fs {

class Path; // Forward declaration

class IterDir {
	const Path &parent;
	DIR *dir;
	struct dirent *entry;

public:
	IterDir(const Path &parent, const String &path, bool end = false) : parent(parent) {
		dir = nullptr;
		entry = nullptr;

		if (!end && connected()) {
			dir = opendir(_path(path).c_str());
			if (dir) {
				entry = readdir(dir);
			}
		}
	}

	~IterDir() {
		if (dir && connected()) {
			closedir(dir);
		}
	}

	bool operator!=(const IterDir &other) const {
		return entry != nullptr;
	}

	IterDir &operator++() {
		if (dir) {
			entry = readdir(dir);
		}
		return *this;
	}

	const Path operator*() const;
};

class Path {
	String path;

public:
	Path(const String &path) {
		if (path.endsWith("/")) {
			this->path = path.substring(0, path.length() - 1); // Remove trailing slash
		} else {
			this->path = path;
		}
	}

	bool exists() const {
		if (!fs::connected()) {
			return false;
		}
		struct stat st;
		return stat(_path(path).c_str(), &st) == 0;
	}

	bool isDir() const {
		if (!fs::connected()) {
			return false;
		}
		struct stat st;
		if (stat(_path(path).c_str(), &st) != 0) {
			return false;
		}
		return S_ISDIR(st.st_mode);
	}

	bool isFile() const {
		if (!fs::connected()) {
			return false;
		}
		struct stat st;
		if (stat(_path(path).c_str(), &st) != 0) {
			return false;
		}
		return S_ISREG(st.st_mode);
	}

	Path operator/(const String &subPath) const {
		return Path(path + "/" + subPath);
	}

	Path &operator/=(const String &subPath) {
		path += "/" + subPath;
		return *this;
	}

	const String &str() const {
		return path;
	}

	const char *c_str() const {
		return path.c_str();
	}

	String name() const {
		int lastSlash = path.lastIndexOf('/');
		if (lastSlash == -1) {
			return path;
		}
		return path.substring(lastSlash + 1);
	}

	String stem() const {
		String n = name();
		int dotIndex = n.lastIndexOf('.');
		if (dotIndex == -1) {
			return n;
		}
		return n.substring(0, dotIndex);
	}

	String ext() const {
		String n = name();
		int dotIndex = n.lastIndexOf('.');
		if (dotIndex == -1) {
			return ""; // No extension
		}
		n = n.substring(dotIndex + 1);
		n.toLowerCase();
		return n;
	}

	String parent() const {
		int lastSlash = path.lastIndexOf('/');
		if (lastSlash == -1) {
			return ""; // No parent directory
		}
		return path.substring(0, lastSlash);
	}

	String read() const {
		if (!connected()) {
			return "";
		}

		auto file = fopen(_path(path).c_str(), "r");
		if (!file) {
			logger::error("Failed to open file for reading: " + path);
			return "";
		}

		String content;
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), file)) {
			content += buffer;
		}

		fclose(file);
		return content;
	}

	std::vector<String> readlines() const {
		if (!connected()) {
			return {};
		}

		std::vector<String> lines;
		String data = read();
		if (data.isEmpty()) {
			return lines; // Return empty vector if no data
		}

		int index = 0;
		while (index < data.length()) {
			int nextIndex = data.indexOf('\n', index);
			if (nextIndex == -1) {
				lines.push_back(data.substring(index));
				break; // No more newlines, add the rest
			}
			lines.push_back(data.substring(index, nextIndex));
			index = nextIndex + 1; // Move past the newline
		}

		return lines;
	}

	bool write(const String &data, bool append = false) const {
		if (!connected()) {
			return false;
		}

		auto file = fopen(_path(path).c_str(), append ? "a" : "w");
		if (!file) {
			logger::error("Failed to open file for writing: " + path);
			return false;
		}

		size_t written = fwrite(data.c_str(), 1, data.length(), file);
		fclose(file);

		if (written != data.length()) {
			logger::error("Failed to write all data to file: " + path);
			return false;
		}

		return true;
	}

	bool write(const std::vector<uint8_t> &data, bool append = false) const {
		if (!connected()) {
			return false;
		}

		auto file = fopen(_path(path).c_str(), append ? "ab" : "wb");
		if (!file) {
			logger::error("Failed to open file for writing: " + path);
			return false;
		}

		size_t written = fwrite(data.data(), 1, data.size(), file);
		fclose(file);

		if (written != data.size()) {
			logger::error("Failed to write all data to file: " + path);
			return false;
		}

		return true;
	}

	bool mkdir(bool exist_ok = false) const {
		if (!connected()) {
			return false;
		}

		if (exists()) {
			if (exist_ok) {
				if (!isDir()) {
					logger::error("Path exists but is not a directory: " + path);
					return false;
				}
				return true;
			}
			logger::error("Directory already exists: " + path);
			return false;
		}

		if (::mkdir(_path(path).c_str(), 0755) != 0) {
			logger::error("Failed to create directory: " + path);
			return false;
		}
		return true;
	}

	int size() const {
		if (!connected()) {
			return -1;
		}

		struct stat st;
		if (stat(_path(path).c_str(), &st) != 0) {
			logger::error("Failed to get file size: " + path);
			return -1;
		}
		return st.st_size;
	}

	FileStream stream() const {
		if (!connected()) {
			logger::error("FileStream is not connected to a USB device.");
			return FileStream();
		}

		auto file = fopen(_path(path).c_str(), "r");
		if (!file) {
			logger::error("Failed to open file for streaming: " + path);
			return FileStream();
		}

		return FileStream(file);
	}

	IterDir begin() const {
		return IterDir(*this, path);
	}

	IterDir end() const {
		return IterDir(*this, path, true);
	}
};

const Path IterDir::operator*() const {
	if (entry) {
		return parent / String(entry->d_name);
	}
	return Path(""); // Return an empty path if no entry
}

} // namespace fs