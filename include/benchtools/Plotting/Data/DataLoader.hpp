#pragma once

#include <benchtools/File/CSVParser.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace benchtools {

namespace plotter {

    struct BenchmarkData {
        std::vector<double> xData, yData;
        std::vector<std::string> labels;
        std::string unit, time_type;
    };

    [[nodiscard]] double extract_duration(const std::string& raw) noexcept;

    [[nodiscard]] std::string extract_col(const std::string& raw) noexcept;

    struct PlotDataLoader {
        [[nodiscard]] static BenchmarkData LoadFromCSV(std::string_view path);
    };

}  // namespace plotter

}  // namespace benchtools