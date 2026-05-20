#pragma once

#include <benchtools/Benchmark/Policy.hpp>

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <benchtools/File/CSVStream.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <string_view>
#include <type_traits>

namespace benchtools {
/**
 * @brief
 *
 */
template <Policy P, class Callable, class... Args>
    requires(std::is_invocable_v<Callable, Args...>)
auto benchmark(uint32_t iterations, time_unit unit, Callable&& callable, Args&&... args) {
    static constexpr auto benchmarkCSVHeaderLength = 3;

    File path_to_save =
        File("benchtools_results") / (format(time_date()) + "_benchmarkresults.csv");

    std::filesystem::create_directories(path_to_save.parent_path());

    CSVStream<benchmarkCSVHeaderLength> CSVResults{path_to_save.c_str(), fileopen::insert,
                                                   "timerid", "type", "dur"};

    std::chrono::duration<double> total{default_duration};

    using Timer_t = std::conditional_t<P == Policy::Wall, benchtools::timers::WallTimer,
                                       benchtools::timers::ClockTimer>;
    Timer_t timer;

    for (uint32_t i{0}; i < iterations; i++) {
        timer.start();
        std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
        timer.stop();

        total += timer.currentElapsed();

        CSVResults.write(std::array<std::string, benchmarkCSVHeaderLength>{
            std::to_string(i), format(P), timer.duration(unit).str()});

        timer.reset();
    }
    return std::to_string((total / iterations).count()) + format(unit);
}
}  // namespace benchtools