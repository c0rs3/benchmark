#pragma once

#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/BaseTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <atomic>

namespace benchtools {

/**
 * @brief Wrapper for BaseTimer, logs with spdlog after either explicitly calling stop or
 * the destructor
 *
 *
 */
class LoggingTimer {
  public:
    /**
     * @brief Construct a new Logging Timer object
     *
     * @param timer
     * @param unit
     */
    explicit LoggingTimer(BaseTimer& timer, time_unit unit = time_unit::seconds) noexcept
        : m_Timer(&timer), m_Unit(unit) {
        m_Timer->start();
    };

    /**
     * @brief Destroy the Logging Timer object
     *
     */
    ~LoggingTimer() noexcept;

    /**
     * @brief
     *
     */
    void start() noexcept;

    /**
     * @brief
     *
     */
    void stop() noexcept;

    void setUnit(time_unit unit) noexcept;

  private:
    BaseTimer* m_Timer{};
    time_unit m_Unit{time_unit::seconds};
    std::atomic<bool> m_IsStopped{0};
};
}  // namespace benchtools