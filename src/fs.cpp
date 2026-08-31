#include "fs.h"
#include <expected>
#include <fstream>

namespace flox {
std::expected<std::string, FileError> read_file(std::filesystem::path path) {
    std::ifstream file{path};
    if (!file) {
        return std::unexpected(FileError::NotFound);
    }
    auto size = file.seekg(0, std::ios::end).tellg();
    file.seekg(0);
    if (size < 0) {
        return std::unexpected(FileError::ReadFailed);
    }
    std::string content{};
    content.resize(static_cast<size_t>(size));
    if (!file.read(content.data(), size)) {
        return std::unexpected(FileError::ReadFailed);
    }
    return content;
}
} // namespace flox
