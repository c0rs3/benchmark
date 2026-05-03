#include <benchtools/Timers/BaseTimer.hpp>
#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/LoggingTimer.hpp>
#include <benchtools/Timers/ScopedTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

namespace benchtools {
/**
 * @brief Default alias for Timer which is WallTimer
 *
 */
using Timer = WallTimer;
};  // namespace benchtools