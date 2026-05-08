#pragma once

#include <benchtools/Benchmark/Policy.hpp>

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/Core/CSVStream.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
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
[[nodiscard]] auto benchmark(Callable&& callable, int iterations, Args&&... args) {
    CSVStream<3> CSVResults{"result.csv", fileopen::clear, "timerid", "type", "dur"};

    std::chrono::duration<double> total{default_duration};

    using Timer_t = std::conditional_t<P == Policy::Wall, WallTimer, ClockTimer>;
    Timer_t timer;
    for (uint32_t i{0}; i < iterations; i++) {
        timer.start();
        std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
        timer.stop();
        total += timer.currentElapsed();
        CSVResults.write(std::array<std::string, 3>{
            std::to_string(i), format(P), timer.duration(time_unit::milliseconds).str()});
        timer.reset(true);
    }
    return total / iterations;
}
}  // namespace benchtools