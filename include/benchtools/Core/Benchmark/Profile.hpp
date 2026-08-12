#pragma once

#include <benchtools/Core/Benchmark/Policy.hpp>

#include <cstdint>

namespace benchtools::benchmark {
/** @brief Object used for setting up benchmarks */
struct Profile {
    int64_t warmupIterations{2};
    int64_t iterations{10};
    Policy policy{Policy::CPU_Process};
    bool warmup{true};
};
}  // namespace benchtools::benchmark