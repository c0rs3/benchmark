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
    constexpr inline fmode append = std::ios::app | std::ios::out;
    constexpr inline fmode insert = std::ios::trunc | std::ios::out;
    constexpr inline fmode input = std::ios::in;
};  // namespace fileopen

/**
 * @brief Alias for a file's path
 *
 */
using File = std::filesystem::path;

/**
 * @brief Parses a CSV file headers/rows
 */
template <size_t N>
[[nodiscard]] std::array<std::string, N>
parseCSVLine(const std::string& header_line) noexcept {
    std::array<std::string, N> line{};
    std::string line_column{};

    for (uint16_t counter{}; char x : header_line) {
        if (x == ',') {
            line[counter++] = line_column;
            line_column.clear();
            continue;
        }

        line_column += x;
    }

    line[N - 1] = line_column;
    return line;
}

}  // namespace benchtools