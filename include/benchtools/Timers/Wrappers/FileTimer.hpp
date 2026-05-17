#pragma once

#include "benchtools/Core/Time.hpp"
#include "benchtools/File/File.hpp"
#include "benchtools/Timers/BaseTimer.hpp"
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
class FileTimer {
  public:
    explicit FileTimer() = delete;

    /**
     * @brief Construct a new FileTimer object
     *
     * @param timer
     * @param path
     * @param unit
     * @param filemode
     */
    explicit FileTimer(BaseTimer& timer, std::string_view path,
                       fmode filemode = fileopen::insert,
                       time_unit unit = time_unit::seconds) noexcept
        : m_Timer(&timer), m_Logger(path), m_Unit(unit) {}

    /**
     * @brief Logs the timer started with the current time and date and at which line it
     * started
     *
     * @param loc
     */
    void start(std::source_location loc = std::source_location::current()) noexcept {
        m_ID = loc;

        m_Timer->reset();
        m_Timer->start();

        std::stringstream ss;
        ss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << " Started timer";

        m_Logger.Log(ss.str(), LogType::TIMER);
    }

    /**
     * @brief Logs the timer finished with the current time and date and at which line it
     * was started
     *
     * @param loc
     */
    void stop(bool cond = true,
              std::source_location loc = std::source_location::current()) noexcept {
        m_Timer->stop();
        m_Timer->reset_if(cond);

        std::stringstream ss;
        ss << loc.file_name() << ":" << loc.line() << " "
           << "Timer started at: " << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << ", resulted with:" << " "
           << m_Timer->duration(m_Unit).str();

        m_Logger.Log(ss.str(), LogType::TIMER);
    }

    /**
     * @brief Sets the time unit
     *
     * @param unit
     */
    void setUnit(time_unit unit) noexcept { m_Unit = unit; }

  private:
    BaseTimer* m_Timer{};
    FileLogger m_Logger;
    time_unit m_Unit{};
    std::source_location m_ID;
};
};  // namespace benchtools