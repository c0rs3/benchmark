#pragma once

#include <chrono>
#include <cstdint>

namespace benchtools {

using Duration = std::chrono::duration<double>;

enum class ClockType : uint8_t {
    CPU_Process = 0x00,  // CPU time
    CPU_Thread = 0x01,   // CPU time
    Wall = 0x02,         // System  Time
};

}  // namespace benchtools