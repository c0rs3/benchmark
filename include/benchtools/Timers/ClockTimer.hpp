#pragma once
#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/BaseTimer.hpp>

#include <atomic>
#include <chrono>
#include <ctime>

namespace benchtools {
namespace timers {
    /**
     * @brief
     *
     */
    class ClockTimer : public BaseTimer {
      public:
        // ClockTimer(const ClockTimer&) = delete;
        // ClockTimer& operator=(const ClockTimer&) = delete;

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
        void reset() noexcept override;

        /**
         * @brief resets the timer if condition is true
         *
         * @param condition
         */
        void reset_if(bool condition = 1) noexcept override;

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
        [[nodiscard]] std::chrono::duration<double>
        currentElapsed() const noexcept override;

      private:
        std::clock_t mStart{0};
        std::clock_t mEnd{0};
        std::atomic<bool> mRunning{false};
    };

    using CPUTimer = ClockTimer;

}  // namespace timers
}  // namespace benchtools