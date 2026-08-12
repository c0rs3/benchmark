#pragma once

#include <benchtools/Core/File/FileStream.hpp>
#include <benchtools/Logger/FileLogger.hpp>
#include <benchtools/Timer/Timer.hpp>

#include <source_location>
#include <string_view>

namespace benchtools::timer {

/**
 * @brief Logs start() stop() actions with the location it was done as well as the time
 * and duration it took between starting and stopping
 */
template <clock::ChronoClock ClockType>
class FileTimer {
  public:
    FileTimer(std::string_view fileName) : m_Stream(fileName, file::fileopen::append) {};

    ~FileTimer() noexcept(0) {
        if (m_Clock.isRunning()) {
            m_Clock.stop();

            std::ostringstream oss;
            oss << m_Clock.duration();
        }
    }

    FileTimer(const FileTimer&) = delete;
    FileTimer& operator=(const FileTimer&) = delete;
    FileTimer(FileTimer&&) noexcept = delete;
    FileTimer& operator=(FileTimer&&) noexcept = delete;

  public:
    void start(bool shouldReset = 0,
               std::source_location loc = std::source_location::current()) {
        m_Clock.start();
        m_Clock.reset_if(shouldReset);

        m_ID = loc;

        std::ostringstream oss;
        using namespace std::string_literals;
        oss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column() << " "
            << m_ID.function_name() << " Started timer" << std::endl;

        m_Stream.Log(oss.str());
    }

    void stop(std::source_location loc = std::source_location::current()) {
        m_Clock.stop();

        std::ostringstream oss;
        oss << loc.file_name() << ":" << loc.line() << " "
            << "Timer started at: " << m_ID.line() << ":" << m_ID.column() << " "
            << m_ID.function_name() << ", resulted with:" << " " << m_Clock.duration()
            << std::endl;

        m_Stream.Log(oss.str());
    }

    void reset() { m_Clock.reset(); }

  private:
    Timer<ClockType> m_Clock{};
    file::FileLogger m_Stream;
    std::source_location m_ID;
};

}  // namespace benchtools::timer