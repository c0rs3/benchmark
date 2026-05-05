#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>

namespace benchtools {

using File = std::filesystem::path;

class FileStream {
  public:
    explicit FileStream(std::string_view path) noexcept;

    explicit FileStream() noexcept = default;

    void append(std::string_view content) noexcept;

    void clear() noexcept;

  private:
    std::ofstream m_Stream;
    File m_File;
};
}  // namespace benchtools