#pragma once

#include <cassert>
#include <cstdint>
#include <string>

namespace benchtools {

/**
 * @brief Policies for executing benchmarks
 *
 */
enum class Policy : uint8_t {
    CPU,
    Clock,
    Wall,
    System,
};

[[nodiscard]] inline constexpr std::string format(Policy policy) {
    switch (policy) {
        using enum Policy;
    case CPU:
        return "CPU";
    case Clock:
        return "CPU";  // aka clock
    case Wall:
        return "Wall";
    case System:
        return "Wall";  // aka system
    default:
        assert(false && "Unknown Policy!\n");
        return "";
        // C++23...
        // std::unreachable();
    }
}

};  // namespace benchtools