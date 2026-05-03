#pragma once

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Core/LogType.hpp>
#include <benchtools/Loggers/FileLogger.hpp>

#include <source_location>
#include <string_view>

namespace benchtools {

/**
 * @brief
 *
 * @tparam timer_t
 */
template <typename timer_t>
    requires(clock_or_wall_timer<timer_t>)
class FileTimer {
  public:
    explicit FileTimer() = delete;

    explicit FileTimer(timer_t& timer, std::string_view path) noexcept
        : m_Timer(&timer), m_Logger(path) {}

    void start(std::source_location loc = std::source_location::current()) noexcept {
        m_ID = loc;

        m_Timer->reset();
        m_Timer->start();

        std::stringstream ss;
        ss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column()
           << " Started timer";

        m_Logger.Log(ss.str(), LogType::TIMER);
    }

    void stop(std::source_location loc = std::source_location::current()) noexcept {
        m_Timer->stop();

        std::stringstream ss;
        ss << loc.file_name() << ":" << loc.line() << " "
           << "Timer started at: " << m_ID.line() << ":" << m_ID.column()
           << ", resulted with:" << " " << m_Timer->duration().str();

        m_Logger.Log(ss.str(), LogType::TIMER);
    }

  private:
    std::source_location m_ID;
    FileLogger m_Logger;
    timer_t* m_Timer{};
};
};  // namespace benchtools