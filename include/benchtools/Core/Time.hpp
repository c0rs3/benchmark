#pragma once

#include <benchtools/Core/Core.hpp>

#include <cassert>
#include <chrono>
#include <cstdint>
#include <string>

namespace benchtools {

namespace time {

    // default expression for default durations
    inline constexpr auto DEFAULT_DUR{Duration::zero()};
    inline constexpr auto MAX_DUR{Duration::max()};
    inline constexpr auto MIN_DUR{Duration::min()};

    enum class unit : uint8_t {
        months,
        years,
        weeks,
        days,
        hours,
        minutes,
        seconds,
        milliseconds,
        microseconds,
        nanoseconds
    };

    [[nodiscard]] Duration inline durationCast(const Duration& duration,
                                               const time::unit& unit) noexcept {
        using enum time::unit;
        switch (unit) {
        case nanoseconds:
            return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        case microseconds:
            return std::chrono::duration_cast<std::chrono::microseconds>(duration);
        case milliseconds:
            return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        case seconds:
            return std::chrono::duration_cast<std::chrono::seconds>(duration);
        case minutes:
            return std::chrono::duration_cast<std::chrono::minutes>(duration);
        case hours:
            return std::chrono::duration_cast<std::chrono::hours>(duration);
        case days:
            return std::chrono::duration_cast<std::chrono::days>(duration);
        case weeks:
            return std::chrono::duration_cast<std::chrono::weeks>(duration);
        case months:
            return std::chrono::duration_cast<std::chrono::months>(duration);
        case years:
            return std::chrono::duration_cast<std::chrono::years>(duration);
        default:
            return std::chrono::duration_cast<std::chrono::seconds>(duration);
        }
    }

    [[nodiscard]] inline std::chrono::zoned_time<std::chrono::duration<double>>
    currTimeDate() noexcept {
        std::chrono::zoned_time zoned_time{std::chrono::current_zone(),
                                           std::chrono::system_clock::now()};

        zoned_time = {std::chrono::current_zone(),
                      floor<std::chrono::seconds>(zoned_time.get_local_time())};

        return zoned_time;
    };

    [[nodiscard]] std::string inline format(
        std::chrono::zoned_time<std::chrono::duration<double>> time_point) noexcept {
        std::ostringstream oss;
        oss << "[" << std::format("{:%F %T}", time_point) << "]" << "" << std::flush;
        return oss.str();
    }

}  // namespace time
}  // namespace benchtools