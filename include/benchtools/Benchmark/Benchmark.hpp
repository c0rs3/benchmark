#pragma once

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Core/Time.hpp>

#include <benchtools/Timers/BaseTimer.hpp>
#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/ScopedTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/Benchmark/Policy.hpp>
#include <benchtools/Benchmark/Task.hpp>

#include <chrono>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace benchtools {
template <typename Signature_t, typename Policy_t>
    requires(is_policy<Policy_t>)
class Benchmark {
  public:
    explicit Benchmark() noexcept = delete;

    ~Benchmark() noexcept { delete m_Timer; }

    explicit Benchmark(Task<Signature_t>& task) noexcept : m_BenchTask(task) {
        if constexpr (std::is_same_v<Policy_t, WallTimer>) {
            m_Timer = new WallTimer{};
        } else if constexpr (std::is_same_v<Policy_t, ClockTimer>) {
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
        // TODO: somehow make out the time unit of this
        return m_AvgDuration.count();
    }

  private:
    Task<Signature_t> m_BenchTask;
    std::chrono::duration<double> m_AvgDuration;
    Policy_t* m_Timer{};
};
}  // namespace benchtools