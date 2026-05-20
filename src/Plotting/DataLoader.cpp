#include <benchtools/Plotting/Data/DataLoader.hpp>

#include <benchtools/Core/Time.hpp>

#include <string>
#include <vector>

namespace benchtools {
namespace plotter {
    [[nodiscard]] double extract_duration(const std::string& raw) noexcept {
        size_t i{};
        while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
            ++i;
        std::string numberPart = raw.substr(0, i);

        double value = std::stod(numberPart);

        return value;
    }

    [[nodiscard]] std::string extract_col(const std::string& raw) noexcept {
        size_t i{};
        while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
            ++i;

        return raw.substr(i);
    }

    /**
     * @brief Loads the benchmark data from CSV
     *
     * @param path
     * @return BenchmarkData
     */
    [[nodiscard]] BenchmarkData PlotDataLoader::LoadFromCSV(std::string_view path) {
        // CSV rows
        static constexpr auto csv_header_length = 3;
        static constexpr auto unit_col = 2;
        static constexpr auto timer_type_col = 1;
        auto rows = CSVParser<csv_header_length>::getRows(path);

        // Graph data
        std::vector<double> x_data{}, y_data{};
        std::vector<std::string> labels{};

        // Extract the unit type and timer type from the first (value) row
        std::string unit = extract_col(rows[0][unit_col]);
        std::string timer_t = extract_col(rows[0][timer_type_col]);

        for (const auto& row : rows) {

            int id = std::stoi(row[0]);
            x_data.push_back(static_cast<double>(id));

            double dur = extract_duration(row[2]);
            y_data.push_back(dur);

            static std::vector<std::string> label_strings;
            label_strings.push_back(row[0]);
            labels.push_back(label_strings.back());
        }
        return {x_data, y_data, labels, unit, timer_t};
    }

}  // namespace plotter

}  // namespace benchtools