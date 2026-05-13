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
        assert(false && "Unknown Policy!\n");
        // C++23...
        // std::unreachable();
    }
}

};  // namespace benchtools