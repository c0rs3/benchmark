#pragma once

#include <benchtools/Core/Core.hpp>

#include <atomic>
#include <concepts>

namespace benchtools {

namespace clock {

    /** @brief Contract for chrono compatible clocks */
    template <typename T>
    concept ChronoClock = requires {
        typename T::duration;
        typename T::time_point;
        { T::now() } -> std::same_as<typename T::time_point>;
    };
}  // namespace clock

namespace timer {
    /**
     * @brief Simple timer for timing
     */
    template <clock::ChronoClock ClockType>
    class Timer {
      public:
        using duration_t = ClockType::duration;
        using time_point_t = ClockType::time_point;

      public:
        void start() {
            m_State.exchange(1);
            m_Start = ClockType::now();
        }

        void stop() {
            if (m_State.load()) {
                m_State.exchange(0);
                m_End = ClockType::now();
            }
        }

        void reset() {
            m_Start = {};
            m_End = {};
            m_State.exchange(0);
        }

        void reset_if(bool cond) {
            if (cond) reset();
        }

        [[nodiscard]] duration_t duration() { return m_End - m_Start; }

        [[nodiscard]] duration_t currentElapsed() { return ClockType::now() - m_Start; }

        [[nodiscard]] bool isRunning() const { return m_State; }

      private:
        [[nodiscard]] static time_point_t now() { return ClockType::now(); }

      private:
        time_point_t m_Start{};
        time_point_t m_End{};
        std::atomic<bool> m_State{0};
    };

}  // namespace timer
}  // namespace benchtools
