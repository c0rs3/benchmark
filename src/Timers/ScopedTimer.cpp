#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/BaseTimer.hpp>
#include <benchtools/Timers/ScopedTimer.hpp>

#include <chrono>

namespace benchtools {

ScopedTimer::ScopedTimer(BaseTimer& timer) noexcept : m_Timer(&timer) {
    this->getTimer().reset(true);
    this->getTimer().start();
}

ScopedTimer::~ScopedTimer() noexcept {
    this->getTimer().stop();
}

Duration ScopedTimer::duration(time_unit durationType) noexcept {
    return this->getTimer().duration();
}

BaseTimer& ScopedTimer::getTimer() noexcept {
    return *m_Timer;
}

std::chrono::duration<double> ScopedTimer::currentElapsed() noexcept {
    return default_duration;
}
};  // namespace benchtools
