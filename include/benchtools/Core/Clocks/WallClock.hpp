#pragma once

#include <chrono>

namespace benchtools {
namespace clock {
    /** @brief Basic chrono wrapper for Wall time */
    class WallClock {
      public:
        using duration = std::chrono::nanoseconds;
        using rep = duration::rep;
        using period = duration::period;
        using clock = std::chrono::high_resolution_clock;
        using time_point = std::chrono::time_point<WallClock>;

        [[nodiscard]] static time_point now() {
            return time_point(clock::now().time_since_epoch());
        }
    };
};  // namespace clock
};  // namespace benchtools
