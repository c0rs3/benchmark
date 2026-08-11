#pragma once

#include "benchtools/Core/Core.hpp"
#include <benchtools/Timer/Timer.hpp>

#include <benchtools/Logger/Logger.hpp>

#include <source_location>
#include <sstream>

namespace benchtools::timer {

template <clock::ChronoClock ClockType>
class LoggingTimer {
  public:
    LoggingTimer() = default;

    ~LoggingTimer() noexcept {
        if (m_Clock.isRunning()) {
            m_Clock.stop();

            std::ostringstream oss;
            oss << m_Clock.duration();

            BENCHTOOLS_TRACE(oss.str());
        }
    }

    LoggingTimer(const LoggingTimer&) = delete;
    LoggingTimer& operator=(const LoggingTimer&) = delete;
    LoggingTimer(LoggingTimer&&) = delete;
    LoggingTimer& operator=(LoggingTimer&&) = delete;

  public:
    benchtoolStatus start(bool shouldReset = 0,
                          std::source_location loc = std::source_location::current()) {
        m_Clock.start();
        m_Clock.reset_if(shouldReset);

        m_ID = loc;

        std::stringstream ss;
        using namespace std::string_literals;
        ss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << " Started timer";
        BENCHTOOLS_TRACE(ss.str());
        return 0;
    }

    benchtoolStatus stop(std::source_location loc = std::source_location::current()) {
        m_Clock.stop();

        std::stringstream ss;
        ss << loc.file_name() << ":" << loc.line() << " "
           << "Timer started at: " << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << ", resulted with:" << " " << m_Clock.duration();

        BENCHTOOLS_TRACE(ss.str());

        return 0;
    }

  private:
    Timer<ClockType> m_Clock;
    std::source_location m_ID;
};
}  // namespace benchtools::timer