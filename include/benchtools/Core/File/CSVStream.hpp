#pragma once

#include <benchtools/Core/File/FileStream.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools::file {

using CSVLine = std::vector<std::string>;

using CSVContent = std::vector<CSVLine>;

[[nodiscard]] inline std::vector<std::string>
parseCSVLine(const std::string& line) noexcept {
    std::vector<std::string> res;
    std::string line_column{};

    for (const auto& x : line) {
        if (x == ',') {
            res.push_back(line_column);
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

class CSVStream {

  public:
    explicit CSVStream() noexcept = delete;

    template <class... Args>
    explicit CSVStream(std::string_view path, Args&&... args)
        : m_Stream(path), m_Headers{std::forward<Args>(args)...} {
        for (size_t l_counter{1}; const auto& header : m_Headers) {
            m_Stream.append(header);

            if (l_counter++ < m_Headers.size()) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    explicit CSVStream(std::string_view path, CSVLine headers)
        : m_Stream(path), m_Headers(headers) {

        for (size_t l_counter{1}; const auto& header : m_Headers) {
            m_Stream.append(std::string(header));

            if (l_counter++ < m_Headers.size()) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    template <class... Args>
    void write(Args&&... args) {
        if (!m_Stream) {
            return;
        }
        CSVLine temp = {(args)...};
        if (temp.size() != m_Headers.size()) {
            return;
        }

        for (size_t l_counter{1}; const auto& content : temp) {
            m_Stream.append(std::string(content));

            if (l_counter++ < m_Headers.size()) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    void write(CSVLine contents) {
        if (!m_Stream) {
            return;
        }

        if (contents.size() != m_Headers.size()) {
            return;
        }

        for (size_t l_counter{1}; const auto& content : contents) {
            m_Stream.append(std::string(content));

            if (l_counter++ < m_Headers.size()) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    [[nodiscard]] static file::CSVLine getHeaders(std::string_view path) {
        file::FileIStream m_Stream{path};

        auto&& lines = m_Stream.read();

        if (lines.empty()) return {};  // or throw

        return file::parseCSVLine(lines.front());
    }

    [[nodiscard]] static file::CSVContent getRows(std::string_view path) {
        file::FileIStream m_Stream{path};
        auto&& lines = m_Stream.read();

        if (lines.empty()) return {};  // or throw

        file::CSVContent m_Content{};
        for (size_t i{1}; i < lines.size(); i++)
            m_Content.emplace_back(file::parseCSVLine(lines[i]));

        return m_Content;
    }

  private:
    file::FileOStream m_Stream{};
    std::size_t N;
    std::vector<std::string> m_Headers{};
};
};  // namespace benchtools::file