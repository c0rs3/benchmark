#pragma once

#include "benchtools/Core/Clocks/CPUClock.hpp"
#include "benchtools/Core/Clocks/WallClock.hpp"
#include "benchtools/Core/Core.hpp"
#include "benchtools/Timer/Timer.hpp"

#include <algorithm>
#include <benchtools/Benchmark/Policy.hpp>
#include <benchtools/Benchmark/Profile.hpp>

#include <benchtools/Core/File.hpp>
#include <benchtools/Core/File/CSV/CSVStream.hpp>
#include <benchtools/Core/Time.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

namespace benchtools::benchmark {

class BenchmarkResult {
  public:
    BenchmarkResult(std::vector<Duration>&& durations, const Profile& profile)
        : durations(durations), profile(profile) {
        std::for_each(std::begin(durations), std::end(durations), [this](auto& dur) {
            sumDuration += dur;
            if (maxDuration < dur) {
                maxDuration = dur;
            }
            if (minDuration > dur) {
                minDuration = dur;
            }
        });
    }
    [[nodiscard]] Duration mean() const noexcept {
        return sumDuration / profile.iterations;
    }

    [[nodiscard]] Duration min() const noexcept { return minDuration; }

    [[nodiscard]] Duration max() const noexcept { return maxDuration; }

    [[nodiscard]] Duration sum() const noexcept { return sumDuration; }

  private:
    std::vector<Duration> durations{};
    const Profile profile;
    Duration minDuration{time::DEFAULT_DUR};
    Duration maxDuration{time::DEFAULT_DUR};
    Duration sumDuration{time::DEFAULT_DUR};
};

template <class Callable, class... Args>
    requires(std::is_invocable_v<Callable, Args...>)
BenchmarkResult benchmarkFunc(const Profile& profile, Callable&& callable,
                              Args&&... args) {
    enum TimerKind : int { CPU_PROCESS = 0x00, CPU_THREAD = 0x01, WALL = 0x02 };

    std::vector<Duration> allDurations(profile.iterations);
    std::size_t minRun{}, maxRun{};

    auto clockVar = std::variant<timer::Timer<clock::CPUClock<clock::ClockType::Process>>,
                                 timer::Timer<clock::CPUClock<clock::ClockType::Thread>>,
                                 timer::Timer<clock::WallClock>>{};

    if (profile.policy == Policy::CPU) {
        if (profile.CPUPolicy == clock::ClockType::Process) {
            clockVar.emplace<TimerKind::CPU_PROCESS>();
        } else if (profile.CPUPolicy == clock::ClockType::Thread) {
            clockVar.emplace<TimerKind::CPU_THREAD>();
        }
    } else if (profile.policy == Policy::Wall) {
        clockVar.emplace<TimerKind::WALL>();
    }

    // TODO: does this guarantee proper cache warming up?
    for (auto warmupRun{0}; warmupRun < profile.warmupCount; warmupRun++) {
        std::invoke(callable, args...);
    }

    for (auto benchRun{0}; benchRun < profile.iterations; benchRun++) {
        {
            std::visit([](auto& timer) { return timer.start(); }, clockVar);
            std::invoke(callable, args...);
            std::visit([](auto& timer) { return timer.stop(); }, clockVar);
        }

        allDurations[benchRun] =
            std::visit([](auto& timer) { return timer.duration(); }, clockVar);

        std::visit([](auto& timer) { return timer.reset(); }, clockVar);
    }

    BenchmarkResult results{std::move(allDurations), profile};

    std::clog << "Total duration: " << results.sum() << std::endl;
    std::clog << "Mean duration: " << results.mean() << std::endl;

    std::clog << "Max duration: " << results.max() << std::endl;
    std::clog << "Min duration: " << results.min() << std::endl;

    file::File fileOut{
        fs::path("benchtools_results") /
        fs::path(time::format(time::time_date()) + "_benchmarkresults.csv")};

    std::filesystem::create_directories(fileOut.parent_path());
    file::CSVStream csvOut{
        fileOut.c_str(), file::fileopen::append, "timerid", "type", "dur", "unit"};

    return results;
}
}  // namespace benchtools::benchmark