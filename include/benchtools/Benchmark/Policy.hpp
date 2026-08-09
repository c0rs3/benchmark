#pragma once

#include <cassert>
#include <cstdint>
#include <string>

namespace benchtools::benchmark {

/**
 * @brief Policies for executing benchmarks
 */
enum class Policy : uint8_t {
    CPU = 0x00,   // CPU time
    Wall = 0x01,  // System  Time
};

[[nodiscard]] inline constexpr std::string format(Policy policy) {
    switch (policy) {
        using enum Policy;
    case CPU:
        return "CPU";
    case Wall:
        return "Wall";
    default:
        assert(false && "Unknown Policy!\n");
        return "";
    }
}

};  // namespace benchtools::benchmark