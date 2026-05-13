#pragma once

#include <benchtools/File/File.hpp>

#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools {

/**
 * @brief File output stream
 *
 */
class FileOStream {
  public:
    explicit FileOStream() noexcept = default;

    explicit FileOStream(std::string_view path, fmode mode = fileopen::append);

    void append(std::string_view content);

  private:
    std::ofstream m_Stream;
    File m_File;
};

/**
 * @brief File input stream
 *
 */
class FileIStream {
  public:
    explicit FileIStream() noexcept = delete;

    FileIStream(std::string_view path) : m_Stream(File(path.data())), m_File(path) {}

    [[nodiscard]] std::vector<std::string> read(uint32_t lines = 0);

  private:
    std::ifstream m_Stream;
    File m_File;
};
}  // namespace benchtools