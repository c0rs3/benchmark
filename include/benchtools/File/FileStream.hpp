#pragma once

#include <benchtools/Core/File.hpp>

#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools {
/**
 * @brief
 *
 */
class FileStream {
  public:
    explicit FileStream() noexcept = default;

    explicit FileStream(std::string_view path, fmode mode = fileopen::append);

    void append(std::string_view content);

  private:
    std::fstream m_Stream;
    File m_File;
};

class FileIStream {
  public:
    explicit FileIStream() noexcept = delete;

    FileIStream(std::string_view path)
        : m_Stream(std::filesystem::path(path.data())), m_File(path) {}

    [[nodiscard]] std::vector<std::string> read(uint32_t lines = 0);

  private:
    std::ifstream m_Stream;
    File m_File;
};
}  // namespace benchtools