#pragma once

#include <benchtools/Core/Core.hpp>

#include <atomic>
#include <chrono>
#include <concepts>

namespace benchtools {

template <typename T>
concept ChronoClock = requires {
    typename T::duration;
    typename T::time_point;
    { T::now() } -> std::same_as<typename T::time_point>;
};

namespace timer {

    template <ChronoClock ClockType>
    class Timer {
      public:
        using duration_t = ClockType::duration;
        using time_point_t = ClockType::time_point;

      public:
        benchtoolStatus start() {
            m_State.exchange(1);
            m_Start = ClockType::now();

            return 0;
        }

        benchtoolStatus stop() {
            if (m_State.load()) {
                m_State.exchange(0);
                m_End = ClockType::now();
                return 0;
            }
            return 1;
        }

        benchtoolStatus reset() {
            m_Start = {};
            m_End = {};
            m_State.exchange(0);
            return 0;
        }

        benchtoolStatus reset_if(bool cond) {
            if (cond) return reset();

            return 1;  // fms
        }

        [[nodiscard]] std::chrono::duration<double> duration() {  // <-- TODO
            return m_End - m_Start;
        }

        [[nodiscard]] std::chrono::duration<double> currentElapsed() {
            return ClockType::now() - m_Start;
        }

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
