// driver/reader.hpp

#ifndef COMPILER_SRC_DRIVER_READER_HPP
#define COMPILER_SRC_DRIVER_READER_HPP

#include <cerrno>
#include <expected>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>

#include "support/primitives.hpp"

namespace compiler::driver {

	enum class ReaderError : u8 {
		FileNotFound,
		InvalidFormat,
		IOError,
	};

	using ReaderResult = std::expected<std::string, ReaderError>;

	inline ReaderResult read_file(const std::string& filename) {
		errno = 0;
		std::ifstream file(filename);
		if (!file.is_open()) {
			if (errno == ENOENT) {
				return std::unexpected(ReaderError::FileNotFound);
			}
			return std::unexpected(ReaderError::IOError);
		}

		try {
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			if (file.bad()) {
				return std::unexpected(ReaderError::IOError);
			}

			return content;
		} catch (const std::ios_base::failure&) {
			return std::unexpected(ReaderError::IOError);
		}
	}

} // namespace compiler::driver

#endif // COMPILER_SRC_DRIVER_READER_HPP
