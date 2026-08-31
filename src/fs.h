#ifndef fs_h
#define fs_h

#include <expected>
#include <filesystem>

enum class FileError {
    NotFound,
    ReadFailed,
};

namespace flox {
std::expected<std::string, FileError> read_file(std::filesystem::path path);
}

#endif
