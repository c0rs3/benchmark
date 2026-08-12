#pragma once

#include <cassert>
#include <string>

namespace benchtools::benchmark {

enum Policy : int {
    CPU_Process = 0x00,  // CPU time (PROCESS)
    CPU_Thread = 0x01,   // CPU time (THREAD)
    Wall = 0x02,         // System  Time
};

/**
 * @brief returns a string translation of the policy
 * @param policy policy
 * @return constexpr std::string 
 */
[[nodiscard]] inline constexpr std::string format(Policy policy) {
    switch (policy) {
        using enum Policy;
    case CPU_Process:
        return "CPU (Process)";
    case CPU_Thread:
        return "CPU (Thread)";
    case Wall:
        return "Wall";
    default:
        assert(false && "Unknown Policy!\n");
        return "";
    }
}

};  // namespace benchtools::benchmark