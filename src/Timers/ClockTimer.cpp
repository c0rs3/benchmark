#include <benchtools/Core/Time.hpp>
#include <benchtools/Timers/ClockTimer.hpp>

namespace benchtools {

void ClockTimer::start() noexcept {
    mStart = std::clock();
    mRunning = true;
}
void ClockTimer::stop() noexcept {
    mEnd = std::clock();
    mRunning = false;
}

void ClockTimer::reset(bool) noexcept {
    mStart = 0;
    mEnd = 0;
}

[[nodiscard]] Duration ClockTimer::duration(time_unit durationType) const noexcept {
    clock_t end = mRunning ? std::clock() : mEnd;
    return getDuration(this->currentElapsed(), durationType);
}

[[nodiscard]] std::chrono::duration<double> ClockTimer::currentElapsed() const noexcept {
    return std::chrono::duration<double>(static_cast<double>(std::clock() - mStart) /
                                         CLOCKS_PER_SEC);
};

}  // namespace benchtools