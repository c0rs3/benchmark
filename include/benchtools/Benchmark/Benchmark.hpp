#pragma once

#include "benchtools/Core/Time.hpp"
#include <benchtools/Benchmark/Policy.hpp>

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/File/CSVStream.hpp>

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
[[nodiscard]] auto benchmark(Callable&& callable, int iterations,
                             time_unit unit = time_unit::seconds, Args&&... args) {
    std::string&& filename = format(time_date()) + "_results.csv";
    CSVStream<3> CSVResults{filename, fileopen::insert, "timerid", "type", "dur"};

    std::chrono::duration<double> total{default_duration};

    using Timer_t = std::conditional_t<P == Policy::Wall, WallTimer, ClockTimer>;
    Timer_t timer;

    for (uint32_t i{0}; i < iterations; i++) {
        timer.start();
        std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
        timer.stop();

        total += timer.currentElapsed();

        CSVResults.write(std::array<std::string, 3>{std::to_string(i), format(P),
                                                    timer.duration(unit).str()});

        timer.reset(true);
    }
    return (total / iterations).count();
}
}  // namespace benchtools