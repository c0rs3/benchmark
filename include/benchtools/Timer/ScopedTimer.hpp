#pragma once

#include <benchtools/Core/Core.hpp>

#include <benchtools/Timer/Timer.hpp>

namespace benchtools::timer {

/**
 * @brief Binds a Timer to start on construction and stop on destruction
 * @tparam ClockType clock type
 */
template <clock::ChronoClock ClockType>
class ScopedTimer {
  public:
    ScopedTimer() = delete;

    explicit ScopedTimer(Timer<ClockType>& clock) noexcept : m_Clock(clock) {
        m_Clock.start();
    }

    ~ScopedTimer() noexcept(0) { m_Clock.stop(); }

  private:
    Timer<ClockType>& m_Clock;
};

}  // namespace benchtools::timer