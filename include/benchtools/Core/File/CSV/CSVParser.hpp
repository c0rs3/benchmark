#pragma once

#include "benchtools/Core/File.hpp"
#include <benchtools/Core/File/CSV/CSVStream.hpp>

#include <cstddef>
#include <string_view>
#include <vector>

namespace benchtools::file {

struct CSVParser {

    [[nodiscard]] static file::CSVLine getHeaders(std::string_view path) {
        file::FileIStream m_Stream{path};

        auto&& lines = m_Stream.read();

        return file::parseCSVLine(lines[0]);
    }

    [[nodiscard]] static file::CSVContent getRows(std::string_view path) {
        file::FileIStream m_Stream{path};
        auto&& lines = m_Stream.read();

        file::CSVContent m_Content{};
        for (size_t i{1}; i < lines.size(); i++)
            m_Content.emplace_back(file::parseCSVLine(lines[i]));

        return m_Content;
    }
};
}  // namespace benchtools::file