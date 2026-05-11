#include <benchtools/Plotting/Data/DataLoader.hpp>

#include <benchtools/Core/Time.hpp>

#include <algorithm>

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

    [[nodiscard]] std::string extract_unit(const std::string& raw) noexcept {
        size_t i{};
        while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
            ++i;
        std::string unitPart = raw.substr(i);

        unitPart.erase(unitPart.begin(),
                       std::find_if(unitPart.begin(), unitPart.end(),
                                    [](char ch) { return !std::isspace(ch); }));
        // std::transform(unitPart.begin(), unitPart.end(), unitPart.begin(),
        //                [](char c) { return std::tolower(c); });

        return unitPart;
    }

    [[nodiscard]] std::pair<std::pair<std::vector<double>, std::vector<double>>,
                            std::vector<std::string>>
    DataLoader::LoadFromCSV(std::string_view path) noexcept {
        // CSV rows
        std::vector<std::array<std::string, 3>> rows = CSVParser<3>::getRows(path);

        // Graph data
        std::vector<double> x_data{}, y_data{};
        std::vector<std::string> labels{};

        for (const auto& row : rows) {
            int id = std::stoi(row[0]);
            x_data.push_back(static_cast<double>(id));

            double dur = extract_duration(row[2]);
            y_data.push_back(dur);

            static std::vector<std::string> label_strings;
            label_strings.push_back(row[0]);
            labels.push_back(label_strings.back());
        }
        return {{x_data, y_data}, labels};
    }

}  // namespace plotter

}  // namespace benchtools