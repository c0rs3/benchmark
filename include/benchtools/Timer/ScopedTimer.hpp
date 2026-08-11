#pragma once

#include <benchtools/Core/Core.hpp>

#include <benchtools/Timer/Timer.hpp>

namespace benchtools::timer {

template <clock::ChronoClock ClockType>
class ScopedTimer {
  public:
    ScopedTimer() noexcept = delete;

    explicit ScopedTimer(Timer<ClockType>& clock) noexcept : m_Clock(clock) {
        m_Clock.start();
    }

    ~ScopedTimer() noexcept(0) { m_Clock.stop(); }

  private:
    Timer<ClockType>& m_Clock;
};

}  // namespace benchtools::timer