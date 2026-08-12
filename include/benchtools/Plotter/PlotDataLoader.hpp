#pragma once

#include <benchtools/Core/Benchmark/Policy.hpp>
#include <benchtools/Core/Benchmark/Profile.hpp>
#include <benchtools/Core/Benchmark/Result.hpp>

#include <benchtools/Core/Core.hpp>
#include <benchtools/Core/File/XMLStream.hpp>
#include <benchtools/Core/Time.hpp>

#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools::plotter {

// TODO: modals for loading data properly and get rid of this mess
struct PlotData {
    static benchmark::Result loadData(std::string_view path) {
        benchmark::Result parsedResults;

        auto headers = file::XMLStream::getHeaders(path);
        if (headers.size() != 3)
            throw std::runtime_error("Header field size does not match!\n");

        // Make sure the headers match
        static constexpr std::string expectedHeaders[3] = {"runid", "dur", "unit"};
        for (auto headerIndex{0}; const auto& field : headers)
            if (field != expectedHeaders[headerIndex++])
                throw std::runtime_error("Header name does not match at header index: " +
                                         std::to_string(headerIndex));

        auto metadata = file::XMLStream::getMetadata(path);

        // iteration count
        const auto&& iterations = std::stoi(metadata[1].second);

        // policy of the benchmark
        benchmark::Policy policy;
        if (metadata[2].second == "CPU (Process)")
            policy = benchmark::Policy::CPU_Process;
        else if (metadata[2].second == "CPU (Thread)")
            policy = benchmark::Policy::CPU_Thread;
        else if (metadata[2].second == "Wall")
            policy = benchmark::Policy::Wall;

        // warmup stuff
        const auto&& warmupEnabled = metadata[3].second == "true" ? true : false;
        const auto&& warmupIterations = std::stoi(metadata[4].second);

        parsedResults.getProfile() =
            benchmark::Profile{warmupIterations, iterations, policy, warmupEnabled};

        //
        // Parsing the data
        //
        struct ParsedDuration {
            std::chrono::nanoseconds nanos;
            time::unit unit;
        };

        // A workaround for parsing each duration since double -> chrono::duration is not
        // possible
        const auto extractDurationFromString =
            [](const std::string& durString,
               const std::string& unitString) -> ParsedDuration {
            const double raw = std::stod(durString);

            if (unitString == "ns") {
                return {std::chrono::nanoseconds(static_cast<int64_t>(raw)),
                        time::unit::nanoseconds};
            } else if (unitString == "µs") {
                const auto temp = std::chrono::duration<double, std::micro>(raw);
                return {std::chrono::duration_cast<std::chrono::nanoseconds>(temp),
                        time::unit::microseconds};
            } else if (unitString == "ms") {
                const auto temp = std::chrono::duration<double, std::milli>(raw);
                return {std::chrono::duration_cast<std::chrono::nanoseconds>(temp),
                        time::unit::milliseconds};
            } else if (unitString == "s") {
                const auto temp = std::chrono::duration<double>(raw);
                return {std::chrono::duration_cast<std::chrono::nanoseconds>(temp),
                        time::unit::seconds};
            }
            throw std::invalid_argument("unknown unit: " + unitString);
        };

        std::vector<Duration> durations(parsedResults.getProfile().iterations);

        auto data = file::XMLStream::getRows(path);

        for (const auto& entries : data) {
            auto runID = std::move(std::stoi(entries[0]));

            auto pentry = extractDurationFromString(entries[1], entries[2]);

            durations[runID] = pentry.nanos;
        }

        parsedResults.setDurations(std::move(durations));
        parsedResults.deduceMinMaxSum();

        return parsedResults;
    }
};
}  // namespace benchtools::plotter