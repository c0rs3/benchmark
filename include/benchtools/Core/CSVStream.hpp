#pragma once

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Core/FileStream.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace benchtools {

template <size_t N>
class CSVStream {

  public:
    explicit CSVStream() noexcept = delete;

    /**
     * @brief
     *
     * @tparam Args the headers of the CSV file
     */
    template <typename... Args>
        requires(sizeof...(Args) == N) && (convertible_to_string<Args> && ...)
    explicit CSVStream(std::string_view path, benchtools::fmode fmode = fileopen::append,
                       Args&&... args) noexcept
        : m_Stream(path, fmode), m_Headers{std::forward<Args>(args)...} {
        uint8_t l_counter{1};
        for (const auto& header : m_Headers) {
            m_Stream.append(std::string(header));
            if (l_counter++ < N) {
                m_Stream.append(",");
            }
        }
        m_Stream.append("\n");
    };

    /**
     * @brief
     *
     * @tparam L
     */
    template <size_t L>
        requires(L == N)
    explicit CSVStream(std::string_view path,
                       std::array<std::string_view, L> headers) noexcept
        : m_Stream(path), m_Headers(headers) {
        uint8_t l_counter{1};
        for (const auto& header : m_Headers) {
            m_Stream.append(std::string(header));
            if (l_counter++ < N) {
                m_Stream.append(",");
            }
        }
        m_Stream.append("\n");
    };

    /**
     * @brief
     *
     * @tparam Args
     */
    template <typename... Args>
        requires(sizeof...(Args) == N) && (convertible_to_string<Args> && ...)
    void write(Args&&... args) noexcept {
        std::array<std::string, N> temp = {(args)...};
        uint8_t l_counter{1};
        for (const auto& content : temp) {
            m_Stream.append(std::string(content));
            if (l_counter++ < N) {
                m_Stream.append(",");
            }
        }
        m_Stream.append("\n");
    };

    /**
     * @brief
     *
     * @tparam L must be equal to header size of CSV Handler
     */
    template <typename str_t, size_t L>
        requires(N == L) && (convertible_to_string<str_t>)
    void write(std::array<str_t, L> contents) noexcept {
        uint8_t l_counter{1};
        for (const auto& content : contents) {
            m_Stream.append(std::string(content));
            if (l_counter++ < N) {
                m_Stream.append(",");
            }
        }
        m_Stream.append("\n");
    };

  private:
    FileStream m_Stream{};
    std::array<std::string, N> m_Headers{};
};
};  // namespace benchtools