
namespace benchtools {

class ClockTimer;

class WallTimer;

/**
 * @brief Policies for executing benchmarks
 *
 */
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
};  // namespace benchtools