#pragma once

#include <benchtools/Loggers/FileLogger.hpp>
#include <benchtools/Timers/BaseTimer.hpp>

#include <source_location>
#include <string_view>

namespace benchtools {
namespace timers {

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
         * @brief Logs the timer started with the current time and date and at which line
         * it started
         *
         * @param loc
         */
        void start(std::source_location loc = std::source_location::current()) noexcept;

        /**
         * @brief Logs the timer finished with the current time and date and at which line
         * it was started
         *
         * @param loc
         */
        void stop(bool cond = true,
                  std::source_location loc = std::source_location::current()) noexcept;

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
};  // namespace timers
};  // namespace benchtools