#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/WallTimer.hpp>

namespace benchtools {

void WallTimer::start() noexcept {
    m_Start = std::chrono::high_resolution_clock::now();
    m_Running = true;
}

void WallTimer::stop() noexcept {
    if (m_Running) {
        m_ElapsedTime += std::chrono::high_resolution_clock::now() - m_Start;
        m_Running = false;
    }
}

void WallTimer::reset() noexcept {
    m_ElapsedTime = default_duration;
    m_Running = false;
}

void WallTimer::reset_if(bool cond) noexcept {
    if (cond) m_ElapsedTime = default_duration;
    m_Running = false;
}

Duration WallTimer::duration(time_unit durationType) const noexcept {
    return getDuration(m_ElapsedTime, durationType);
}

std::chrono::duration<double> WallTimer::currentElapsed() const noexcept {
    return std::chrono::high_resolution_clock::now() - m_Start;
}

}  // namespace benchtools