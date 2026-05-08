#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <iostream>

namespace benchtools {
/**
 * @brief type for file open modes
 *
 */
using fmode = std::ios_base::openmode;

namespace fileopen {
    constexpr inline fmode&& append = std::ios::app | std::ios::out;
    constexpr inline fmode&& insert = std::ios::trunc | std::ios::out;
    constexpr inline fmode input = std::ios::in;
};  // namespace fileopen

/**
 * @brief Alias for a file's path
 *
 */
using File = std::filesystem::path;
template <size_t N>
[[nodiscard]] std::array<std::string, N>
parseHeaders(const std::string& header_line) noexcept {
    std::array<std::string, N> res{};
    std::string header_col{};
    for (uint16_t counter{}; char x : header_line) {
        if (x == ',') {
            res[counter++] = header_col;
            header_col.clear();
            continue;
        }
        header_col += x;
    }
    res[N - 1] = header_col;
    return res;
}

}  // namespace benchtools