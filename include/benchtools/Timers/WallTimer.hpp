#pragma once

#include <benchtools/Core/Time.hpp>

#include <benchtools/Timers/BaseTimer.hpp>

#include <atomic>

namespace benchtools {

/**
 * @brief Fundamental timer used for tracking wall time
 *
 */
class WallTimer : public BaseTimer {
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    friend class LoggingTimer;
    friend class ScopedTimer;

  public:
    /**
     * @brief Construct a new Wall Timer object
     *
     */
    explicit WallTimer() noexcept = default;

    /**
     * @brief Destroy the Wall Timer object
     *
     */
    virtual ~WallTimer() noexcept override = default;

    /**
     * @brief
     *
     */

    virtual void start() noexcept override;
    /**
     * @brief
     *
     */
    virtual void stop() noexcept override;

    /**
     * @brief
     *
     * @param reset
     */
    virtual void reset(bool reset = 0) noexcept override;

    /**
     * @brief
     *
     * @param durationType
     * @return Duration
     */
    [[nodiscard]] virtual Duration
    duration(time_unit durationType = time_unit::seconds) const noexcept override;

    [[nodiscard]] virtual std::chrono::duration<double>
    currentElapsed() const noexcept override;
  private:
    time_point m_Start;
    std::chrono::duration<double> m_ElapsedTime{default_duration};
    std::atomic<bool> m_Running{0};
};

}  // namespace benchtools