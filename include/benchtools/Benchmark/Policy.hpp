
#include <cstdint>
namespace benchtools {

class ClockTimer;

class WallTimer;

/**
 * @brief Policies for executing benchmarks
 *
 */
enum class Policy : uint8_t {
    CPU,
    Wall,
    System,
};
#if 0
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