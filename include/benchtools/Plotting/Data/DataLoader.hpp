#pragma once

#include <benchtools/File/CSVParser.hpp>

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace benchtools {

namespace plotter {

    [[nodiscard]] double extract_duration(const std::string& raw) noexcept;

    [[nodiscard]] std::string extract_unit(const std::string& raw) noexcept;

    struct DataLoader {
        [[nodiscard]] static std::pair<
            std::pair<std::vector<double>, std::vector<double>>,
            std::pair<std::vector<std::string>, std::string>>
        LoadFromCSV(std::string_view path) noexcept;
    };

}  // namespace plotter

}  // namespace benchtools