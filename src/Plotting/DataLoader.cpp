#include <benchtools/Plotting/Data/DataLoader.hpp>

#include <benchtools/Core/Time.hpp>

#include <algorithm>
#include <string>

namespace benchtools {
namespace plotter {

    // TODO: refactor these two functions
    [[nodiscard]] double extract_duration(const std::string& raw) noexcept {
        size_t i{};
        while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
            ++i;
        std::string numberPart = raw.substr(0, i);

        double value = std::stod(numberPart);

        return value;
    }

    [[nodiscard]] std::string extract_unit(const std::string& raw) noexcept {
        size_t i{};
        while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
            ++i;
        std::string unitPart = raw.substr(i);

        unitPart.erase(unitPart.begin(),
                       std::find_if(unitPart.begin(), unitPart.end(),
                                    [](char ch) { return !std::isspace(ch); }));
        return unitPart;
    }

    [[nodiscard]] BenchmarkData DataLoader::LoadFromCSV(std::string_view path) noexcept {
        // CSV rows
        // TODO: add a "naming"
        static constexpr auto csv_header_length = 3;
        static constexpr auto unit_col = 2;
        static constexpr auto timer_type_col = 1;
        std::vector<std::array<std::string, 3>> rows = CSVParser<3>::getRows(path);

        // Graph data
        std::vector<double> x_data{}, y_data{};
        std::vector<std::string> labels{};

        // Extract the unit type and timer type from the first (value) row
        std::string unit = extract_unit(rows[0][unit_col]);
        // TODO: srename this function add an alias
        std::string timer_t = extract_unit(rows[0][timer_type_col]);

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