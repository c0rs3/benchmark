#pragma once

#include <benchtools/Core/File/File.hpp>
#include <benchtools/Logger/LogType.hpp>

#include <benchtools/Core/File/FileStream.hpp>
#include <benchtools/Core/Time.hpp>

#include <array>
#include <sstream>
#include <string_view>

namespace benchtools::file {

class FileLogger {
  public:
    explicit FileLogger(std::string_view path,
                        file::fmode_t = file::fileopen::append) noexcept
        : m_Stream(path) {
        static constexpr auto s_startMessage = "LOGGING SESSION STARTED";

        std::stringstream ss;
        ss << time::format(time::currTimeDate()) << format(log::type::INFO)
           << s_startMessage << "\n";

        m_Stream.append(ss.str());
    };

    ~FileLogger() noexcept {
        static constexpr auto s_endMessage = "LOGGING SESSION ENDED";

        std::stringstream ss;
        ss << time::format(time::currTimeDate()) << format(log::type::INFO)
           << s_endMessage << "\n";

        m_Stream.append(ss.str());
    };

    void Log(std::string_view content, log::type type = log::type::INFO) noexcept {
        std::stringstream ss;

        ss << time::format(time::currTimeDate()) << format(type) << content << "\n";
        m_Stream.append(ss.str());

        ss.flush();
    };

    template <class... Args>
    void Log(Args&&... args, log::type type) {
        std::stringstream ss;
        ss << time::format(time::currTimeDate()) << " " << format(type);

        std::array<std::string_view, sizeof...(args)> contents;
        for (const auto& content : contents) {
            ss << contents;
        }
        ss << "\n";

        m_Stream.append(ss.str());
        ss.flush();
    }

  private:
    file::FileOStream m_Stream{};
};
};  // namespace benchtools::file