#pragma once

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/Benchmark/Policy.hpp>

#include <chrono>
#include <functional>
#include <type_traits>

namespace benchtools {
/**
 * @brief
 *
 * @tparam P
 * @tparam Callable
 * @tparam Args
 * @param callable
 * @param iterations
 * @param args
 * @return auto
 */
template <Policy P, typename Callable, typename... Args>
auto benchmark(Callable&& callable, int iterations, Args&&... args) {
    using Timer_t = std::conditional_t<P == Policy::Wall, WallTimer, ClockTimer>;

    std::chrono::duration<double> total{default_duration};
    Timer_t timer;
    for (int i = 0; i < iterations; ++i) {
        timer.start();
        std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
        timer.stop();
        total += timer.currentElapsed();
        timer.reset(true);
    }
    return total / iterations;
}
}  // namespace benchtools