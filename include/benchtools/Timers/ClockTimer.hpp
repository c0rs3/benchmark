#pragma once
#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/BaseTimer.hpp>

#include <atomic>
#include <chrono>
#include <ctime>

namespace benchtools {

/**
 * @brief
 *
 */
class ClockTimer : public BaseTimer {
  public:
    /**
     * @brief
     *
     */
    void start() noexcept override;

    /**
     * @brief
     *
     */
    void stop() noexcept override;

    /**
     * @brief
     *
     */
    void reset(bool) noexcept override;

    /**
     * @brief
     *
     * @param durationType
     * @return Duration
     */
    [[nodiscard]] Duration
    duration(time_unit durationType = time_unit::seconds) const noexcept override;

    /**
     * @brief
     *
     * @return std::chrono::duration<double>
     */
    [[nodiscard]] std::chrono::duration<double> currentElapsed() const noexcept override;

  private:
    std::clock_t mStart{0};
    std::clock_t mEnd{0};
    std::atomic<bool> mRunning{false};
};

}  // namespace benchtools