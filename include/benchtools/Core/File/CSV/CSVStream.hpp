#pragma once

#include "benchtools/Core/File.hpp"
#include <benchtools/Core/File/FileStream.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools::file {

class CSVStream {

  public:
    explicit CSVStream() noexcept = delete;

    template <class... Args>
    explicit CSVStream(std::string_view path, benchtools::file::fmode fmode = fileopen::append,
                       Args&&... args) noexcept
        : m_Stream(path, fmode), m_Headers{std::forward<Args>(args)...} {
        for (size_t l_counter{1}; const auto& header : m_Headers) {
            m_Stream.append(std::string(header));

            if (l_counter++ < N) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    explicit CSVStream(std::string_view path, CSVLine headers) noexcept
        : m_Stream(path), m_Headers(headers) {

        for (size_t l_counter{1}; const auto& header : m_Headers) {
            m_Stream.append(std::string(header));

            if (l_counter++ < N) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    template <class... Args>
    void write(Args&&... args) noexcept {
        CSVLine temp = {(args)...};
        if (temp.size() != m_Headers.size()) return;

        for (size_t l_counter{1}; const auto& content : temp) {
            m_Stream.append(std::string(content));

            if (l_counter++ < N) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

    void write(CSVLine contents) noexcept {
        if (!m_Stream) return;

        for (size_t l_counter{1}; const auto& content : contents) {
            m_Stream.append(std::string(content));

            if (l_counter++ < N) m_Stream.append(",");
        }

        m_Stream.append("\n");
    };

  private:
    file::FileOStream m_Stream{};
    std::size_t N;
    std::vector<std::string> m_Headers{};
};
};  // namespace benchtools::file