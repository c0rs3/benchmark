#include "benchtools/Core/Clocks/CPUClock.hpp"
#include <benchtools/Benchmark/Policy.hpp>

#include <cstdint>

namespace benchtools::benchmark {
struct Profile {
    Policy policy{Policy::CPU};
    clock::ClockType CPUPolicy{clock::ClockType::Process};
    bool warmupEnabled{true};
    int64_t warmupCount{2};
    int64_t iterations{10};
};
}  // namespace benchtools::benchmark