#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

namespace benchtools {
namespace Policies {
using CPU = benchtools::ClockTimer;
using System = benchtools::WallTimer;
using Wall = benchtools::WallTimer;
}  // namespace Policies
};  // namespace benchtools