#pragma once

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Core/Time.hpp>

#include "benchtools/Timers/ScopedTimer.hpp"
#include <benchtools/Timers/BaseTimer.hpp>
#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/Benchmark/Policy.hpp>
#include <benchtools/Benchmark/Task.hpp>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace benchtools {
template <typename Task_Signature, typename Policy>
    requires(is_policy<Policy>)
class Benchmark {
  public:
    explicit Benchmark() noexcept = delete;

    explicit Benchmark(Task<Task_Signature>& task) noexcept : m_BenchTask(task) {
        if constexpr (std::is_same_v<Policy, WallTimer>) {
            m_Timer = new WallTimer{};
        } else if constexpr (std::is_same_v<Policy, ClockTimer>) {
            m_Timer = new ClockTimer{};
        }
    }

    void run(uint16_t count = 100) {
        for (uint16_t x{0}; x < count; x++) {
            {
                ScopedTimer timer{*m_Timer};
                m_BenchTask.m_Task();
            }
            m_AvgDuration += m_Timer->currentElapsed();
        }
        std::clog << "Total dur:\n";
        std::clog << Duration(getDuration(m_AvgDuration, time_unit::seconds)).str()
                  << std::endl;

        m_AvgDuration = m_AvgDuration / count;

        std::clog << "AVG:\n";
        std::clog << m_AvgDuration << "\n";
    }

    [[nodiscard]] float avgDuration(time_unit unit = time_unit::seconds) const noexcept {
        return m_AvgDuration.count();  // TODO: somehow make out the time unit of this
    }

  private:
    Task<Task_Signature> m_BenchTask;
    std::chrono::duration<double> m_AvgDuration;
    Policy* m_Timer;
};
}  // namespace benchtools