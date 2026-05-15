#pragma once

#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/BaseTimer.hpp>

namespace benchtools {

/**
 * @brief A lock guard like timer that handles a timer object in a scope
 *
 */
class ScopedTimer {
  public:
    /**
     * @brief ScopedTimer must take in a BaseTimer
     *
     */
    explicit ScopedTimer() noexcept = delete;

    /**
     * @brief Construct a new Scoped Timer object
     *
     * @param BaseTimer
     */
    explicit ScopedTimer(BaseTimer& timer) noexcept;

    /**
     * @brief Destroy the Scoped Timer object
     *
     */
    ~ScopedTimer() noexcept;

    /**
     * @brief
     *
     * @param durationType
     * @return Duration
     */
    [[nodiscard]] Duration duration(time_unit durationType = time_unit::seconds) noexcept;

  private:
    /**
     * @brief
     *
     */
    [[nodiscard]] BaseTimer& getTimer() noexcept;

    /**
     * @brief
     *
     * @return std::chrono::duration<double>
     */
    [[nodiscard]] std::chrono::duration<double> currentElapsed() noexcept;

  private:
    BaseTimer* m_Timer{};
};
};  // namespace benchtools