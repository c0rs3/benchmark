#pragma once

#include <benchtools/Core/Time.hpp>

#include <chrono>

namespace benchtools {

/**
 * @brief Base class for timers, used as a blueprint for what a timer should do
 */
class BaseTimer {
  public:
    BaseTimer() = default;

    virtual ~BaseTimer();

    /**
     * @brief Pure virtual function that will define how a Timer should act on start()
     */
    virtual void start() = 0;

    /**
     * @brief Pure virtual function that will define how a Timer should act on stop()
     */
    virtual void stop() = 0;

    /**
     * @brief resets the timer
     *
     */
    virtual void reset() = 0;

    /**
     * @brief resets the timer
     *
     */
    virtual void reset_if(bool cond = true) = 0;

    /**
     * @brief Gets the duration in the respective timeunit
     *
     * @param durationType
     * @return Duration
     */
    [[nodiscard]] virtual Duration
    duration(time_unit durationType = time_unit::seconds) const noexcept {
        return getDuration(default_duration, durationType);
    };

  private:
    /**
     * @brief Pure virtual function for getting elapsed duration since at that moment
     *
     * @return std::chrono::duration<double>
     */
    virtual std::chrono::duration<double> currentElapsed() const noexcept = 0;
};

}  // namespace benchtools