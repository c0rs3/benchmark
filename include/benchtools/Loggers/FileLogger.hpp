#pragma once

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Core/FileStream.hpp>
#include <benchtools/Core/LogType.hpp>
#include <benchtools/Core/Time.hpp>

#include <array>
#include <sstream>
#include <string_view>

namespace benchtools {

class FileLogger {
  public:
    explicit FileLogger(std::string_view path) noexcept;

    void Log(std::string_view content, LogType type = LogType::INFO) noexcept;

    template <typename... Args>
        requires(convertible_to_string<Args> && ...)
    void Log(Args&&... args, LogType type) {
        std::stringstream ss;
        ss << format(time_date()) << format(type);

        std::array<std::string_view, sizeof...(args)> contents;
        for (const auto& content : contents) {
            ss << contents;
        }
        ss << "\n";
        
        m_Stream.append(ss.str());
        ss.flush();
    }

    ~FileLogger() noexcept;

  private:
    FileStream m_Stream{};
};
};  // namespace benchtools