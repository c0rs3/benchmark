#pragma once

#include <benchtools/File/CSVStream.hpp>

#include <cstddef>
#include <string>
#include <string_view>

namespace benchtools {

/**
 * @brief CSVParser object
 *
 * @tparam N
 */
template <size_t N>
struct CSVParser {

    /**
     * @brief Get the Headers from a csv file
     *
     * @param path
     * @return std::array<std::string, N>
     */
    [[nodiscard]] static std::array<std::string, N> getHeaders(std::string_view path) {
        FileIStream m_Stream{path};
        auto&& lines = m_Stream.read();

        std::array<std::string, N> m_Headers{};
        m_Headers = parseHeaders<3>(lines[0]);

        return m_Headers;
    }

    /**
     * @brief Get the Rows from a csv file
     *
     * @param path
     * @return std::vector<std::array<std::string, N>>
     */
    [[nodiscard]] static std::vector<std::array<std::string, N>>
    getRows(std::string_view path) {
        FileIStream m_Stream{path};
        auto&& lines = m_Stream.read();

        std::vector<std::array<std::string, N>> m_Content{};
        for (size_t i{1}; i < lines.size(); i++) {
            m_Content.emplace_back(parseHeaders<3>(lines[i]));
        }

        return m_Content;
    }
};
}  // namespace benchtools