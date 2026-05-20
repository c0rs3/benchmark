#include <benchtools/Core/LogType.hpp>
#include <benchtools/Core/Time.hpp>
#include <benchtools/File/FileStream.hpp>
#include <benchtools/Loggers/FileLogger.hpp>

#include <sstream>
#include <string_view>

namespace benchtools {

FileLogger::FileLogger(std::string_view path, fmode fmode) noexcept {
    static constexpr auto s_startMessage = "LOGGING SESSION STARTED";

    m_Stream = FileOStream{path};

    std::stringstream ss;
    ss << format(time_date()) << format(LogType::INFO) << s_startMessage << "\n";

    m_Stream.append(ss.str());
}

void FileLogger::Log(std::string_view content, LogType type) noexcept {
    std::stringstream ss;

    ss << format(time_date()) << format(type) << content << "\n";
    m_Stream.append(ss.str());

    ss.flush();
}

FileLogger::~FileLogger() noexcept {
    static constexpr auto s_endMessage = "LOGGING SESSION ENDED";

    std::stringstream ss;
    ss << format(time_date()) << format(LogType::INFO) << s_endMessage << "\n";

    m_Stream.append(ss.str());
}
};  // namespace benchtools