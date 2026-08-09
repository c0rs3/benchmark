#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <iostream>
#include <iterator>
#include <vector>

namespace benchtools {

namespace file {

    /**
     * @brief type for file open modes
     */
    using fmode = std::ios_base::openmode;

    namespace fileopen {
        constexpr inline fmode append = std::ios::app | std::ios::out;
        constexpr inline fmode insert = std::ios::trunc | std::ios::out;
        constexpr inline fmode input = std::ios::in;
    };  // namespace fileopen

    /**
     * @brief Alias for a file's path
     */
    using File = std::filesystem::path;

    using CSVLine = std::vector<std::string>;

    using CSVContent = std::vector<CSVLine>;

    [[nodiscard]] inline std::vector<std::string>
    parseCSVLine(const std::string& line) noexcept {
        std::vector<std::string> res{};
        std::string line_column{};

        for (size_t counter{0}; char x : line) {
            if (x == ',') {
                res[counter++] = line_column;
                line_column.clear();
                continue;
            }

            line_column += x;
        }

        // retarded way to insert the last value/header
        res.shrink_to_fit();
        res.resize(std::size(res) + 1);
        res[std::size(res) - 1] = line_column;

        return res;
    }

}  // namespace file
}  // namespace benchtools