#pragma once

#include <cassert>
#include <chrono>
#include <cstdint>
#include <string>

namespace benchtools {

namespace time {

    // default expression for default durations
    inline constexpr auto DEFAULT_DUR = std::chrono::duration<double>::zero();
    inline constexpr auto MAX_DUR = std::chrono::duration<double>::max();
    inline constexpr auto MIN_DUR = std::chrono::duration<double>::min();

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

    [[nodiscard]] inline std::string format(const unit& unit) {
        switch (unit) {
            using enum time::unit;
        case nanoseconds:
            return "ns";
        case microseconds:
            return "µs";
            break;
        case milliseconds:
            return "ms";
            break;
        case seconds:
            return "s";
            break;
        case minutes:
            return "min";
            break;
        case hours:
            return "h";
            break;
        case days:
            return "d";
            break;
        case weeks:
            return "wk";
            break;
        case months:
            return "mo";
            break;
        case years:
            return "y";
            break;
        default:
            return "";
        }
        assert("Unknown timeunit!" && false);
        return {};
    }

    [[nodiscard]] std::chrono::duration<double> inline durationCast(
        const std::chrono::duration<double>& duration, const time::unit& unit) noexcept {
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
    time_date() noexcept {
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