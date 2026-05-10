#pragma once

#include <cstdint>
#include <string>

namespace benchtools {

/**
 * @brief Policies for executing benchmarks
 *
 */
enum class Policy : uint8_t {
    CPU,
    Wall,
    System,
};

[[nodiscard]] inline constexpr std::string format(Policy policy) {
    switch (policy) {
        using enum Policy;
    case CPU:
        return "CPU";
    case Wall:
        return "Wall";
    case System:
        return "System";
    default:
        return "[Unknown policy type]";
    }
    return "";
}

#if 0

class ClockTimer;

class WallTimer;

namespace Policies {
    /**
     * @brief Policy for benchmarking CPU Time
     *
     */
    using CPU = benchtools::ClockTimer;
    /**
     * @brief Policy for benchmarking system (wall) time
     *
     */
    using System = benchtools::WallTimer;
    /**
     * @brief Policy for benchmarking system (wall) time
     *
     */
    using Wall = benchtools::WallTimer;
}  // namespace Policies
#endif
};  // namespace benchtools