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
template <Policy P, typename Callable, typename... Args>
auto benchmark(Callable&& callable, uint32_t iterations,
               time_unit unit = time_unit::seconds, Args&&... args) {
    static constexpr auto csv_header_length = 3;

    File path_to_save = File("results") / (format(time_date()) + "_results.csv");

    std::filesystem::create_directories(path_to_save.parent_path());

    CSVStream<csv_header_length> CSVResults{path_to_save.c_str(), fileopen::insert,
                                            "timerid", "type", "dur"};

    std::chrono::duration<double> total{default_duration};

    using Timer_t = std::conditional_t<P == Policy::Wall, WallTimer, ClockTimer>;
    Timer_t timer;

    for (uint32_t i{0}; i < iterations; i++) {
        timer.start();
        std::invoke(std::forward<Callable>(callable), std::forward<Args>(args)...);
        timer.stop();

        total += timer.currentElapsed();

        CSVResults.write(std::array<std::string, csv_header_length>{
            std::to_string(i), format(P), timer.duration(unit).str()});

        timer.reset(true);
    }
    return std::to_string((total / iterations).count()) + format(unit);
}
}  // namespace benchtools