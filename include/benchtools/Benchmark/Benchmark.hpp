#pragma once

#include <benchtools/Core/Clocks/CPUClock.hpp>
#include <benchtools/Core/Clocks/WallClock.hpp>

#include <benchtools/Core/Benchmark/Result.hpp>
#include <benchtools/Core/Core.hpp>
#include <benchtools/Core/File/XMLStream.hpp>

#include <benchtools/Timer/Timer.hpp>

#include <cctype>
#include <filesystem>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace benchtools::benchmark {

template <class Callable, class... Args>
    requires(std::is_invocable_v<Callable, Args...>)
Result runBenchmark(const Profile& profile, Callable&& callable, Args&&... args) {
    using namespace std::string_literals;
    using TimerKind = Policy;
    namespace fs = std::filesystem;

    auto clockVar = std::variant<timer::Timer<clock::CPUClock<clock::CPU::Process>>,
                                 timer::Timer<clock::CPUClock<clock::CPU::Thread>>,
                                 timer::Timer<clock::WallClock>>{};

    // Construct the correct clock type
    if (profile.policy == Policy::CPU_Process)
        clockVar.emplace<TimerKind::CPU_Process>();
    else if (profile.policy == Policy::CPU_Thread)
        clockVar.emplace<TimerKind::CPU_Thread>();
    else if (profile.policy == Policy::Wall)
        clockVar.emplace<TimerKind::Wall>();

    // Warmup runs
    // TODO: does this guarantee proper cache warming up?
    if (profile.warmup)
        for (auto warmupRun{0}; warmupRun < profile.warmupIterations; warmupRun++)
            std::invoke(callable, args...);

    std::vector<Duration> allDurations(profile.iterations);
    for (auto benchRun{0}; benchRun < profile.iterations; benchRun++) {
        // Time
        {
            std::visit([](auto& timer) { return timer.start(); }, clockVar);
            std::invoke(callable, args...);
            std::visit([](auto& timer) { return timer.stop(); }, clockVar);
        }

        // Insert the run duration
        allDurations[benchRun] =
            std::visit([](auto& timer) { return timer.duration(); }, clockVar);

        // Reset
        std::visit([](auto& timer) { return timer.reset(); }, clockVar);
    }

    Result results{std::move(allDurations), profile};

    std::clog << "Total duration: " << results.sum() << std::endl;
    std::clog << "Mean duration: " << results.mean() << std::endl;

    std::clog << "Max duration: " << results.max() << std::endl;
    std::clog << "Min duration: " << results.min() << std::endl;

    // XML results
    static constexpr auto outFileSuffix{"_benchmarkresults.xml"};
    static constexpr auto outPath{"benchtools_results"};

    file::File XMLResultPath{fs::path(outPath) /
                             fs::path(time::format(time::time_date()) + outFileSuffix)};

    fs::create_directories(XMLResultPath.parent_path());

    auto stream = file::XMLStream{XMLResultPath.c_str(), "runs", "runid", "dur", "unit"};

    // Build metadata
    stream.meta("date", time::format(time::time_date()));
    stream.meta("iterations", std::to_string(profile.iterations));
    stream.meta("policy", format(profile.policy));
    stream.meta("warmupEnabled", profile.warmup ? "true" : "false");
    stream.meta("warmupIterations", std::to_string(profile.warmupIterations));

    // Helper lambda to extract unit and duration
    const auto extractFromDuration =
        [](const Duration& dur) -> std::pair<double, std::string> {
        std::ostringstream oss;
        oss << dur;

        std::string unit{};
        unit.reserve(3);

        const auto&& str = oss.str();
        // extract unit from the other end
        for (auto iter = str.rbegin(); iter != str.rend(); iter++) {
            if (std::isalpha(*iter))
                unit.push_back(*iter);
            else
                break;
        }
        return {dur.count(), unit};
    };

    // min, max, mean, sum of durations
    auto&& durationUnitPair = extractFromDuration(results.min());
    stream.meta("minRecorded",
                std::to_string(durationUnitPair.first) + durationUnitPair.second);

    durationUnitPair = extractFromDuration(results.max());
    stream.meta("maxRecorded",
                std::to_string(durationUnitPair.first) + durationUnitPair.second);

    durationUnitPair = extractFromDuration(results.mean());
    stream.meta("meanRecorded",
                std::to_string(durationUnitPair.first) + durationUnitPair.second);

    durationUnitPair = extractFromDuration(results.sum());
    stream.meta("sumRecorded",
                std::to_string(durationUnitPair.first) + durationUnitPair.second);

    // Write out each duration with it's respective unit
    for (auto runID{0ul}; const auto& run : results.getDurations()) {
        auto&& [dur, unit] = extractFromDuration(run);

        stream.write(file::XMLLine{std::to_string(runID++), std::to_string(dur), unit});
    }

    return results;
}
}  // namespace benchtools::benchmark