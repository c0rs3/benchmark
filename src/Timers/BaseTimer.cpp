#include "benchtools/Core/Time.hpp"
#include <benchtools/Timers/BaseTimer.hpp>

namespace benchtools {
namespace timers {

    BaseTimer::~BaseTimer() {}

    void BaseTimer::start() noexcept {}

    void BaseTimer::stop() noexcept {}

    void BaseTimer::reset() noexcept {}

    void BaseTimer::reset_if(bool) noexcept {}

    std::chrono::duration<double> BaseTimer::currentElapsed() const noexcept {
        return default_duration;
    };
}  // namespace timers
}  // namespace benchtools