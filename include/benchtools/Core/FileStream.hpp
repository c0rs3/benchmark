#pragma once

#include <filesystem>
#include <fstream>
#include <ios>
#include <string_view>

namespace benchtools {

/**
 * @brief type for file open modes
 *
 */
using fmode = std::_Ios_Openmode;

namespace fileopen {
    constexpr inline auto append = std::ios::app | std::ios::out;
    constexpr inline auto clear = std::ios::trunc | std::ios::out;
};  // namespace fileopen

/**
 * @brief Alias for a file's path
 *
 */
using File = std::filesystem::path;

/**
 * @brief
 *
 */
class FileStream {
  public:
    explicit FileStream(std::string_view path, fmode mode = std::ios::app) noexcept;

    explicit FileStream() noexcept = default;

    void append(std::string_view content) noexcept;

    void clear() noexcept;

  private:
    std::ofstream m_Stream;
    File m_File;
};
}  // namespace benchtools