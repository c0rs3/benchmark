#include <benchtools/Core/Time.hpp>

#include <chrono>
#include <format>
#include <ostream>
#include <sstream>
#include <string>

namespace benchtools {

Duration getDuration(const std::chrono::duration<double>& duration,
                     time_unit unit) noexcept {
    switch (unit) {
    case time_unit::nanoseconds:
        return Duration{std::chrono::duration_cast<std::chrono::nanoseconds>(duration)};
    case time_unit::microseconds:
        return Duration{std::chrono::duration_cast<std::chrono::microseconds>(duration)};
    case time_unit::milliseconds:
        return Duration{std::chrono::duration_cast<std::chrono::milliseconds>(duration)};
    case time_unit::seconds:
        return Duration{std::chrono::duration_cast<std::chrono::seconds>(duration)};
    case time_unit::minutes:
        return Duration{std::chrono::duration_cast<std::chrono::minutes>(duration)};
    case time_unit::hours:
        return Duration{std::chrono::duration_cast<std::chrono::hours>(duration)};
    case time_unit::days:
        return Duration{std::chrono::duration_cast<std::chrono::days>(duration)};
    case time_unit::weeks:
        return Duration{std::chrono::duration_cast<std::chrono::weeks>(duration)};
    case time_unit::months:
        return Duration{std::chrono::duration_cast<std::chrono::months>(duration)};
    case time_unit::years:
        return Duration{std::chrono::duration_cast<std::chrono::years>(duration)};
    default:
        return Duration{std::chrono::duration_cast<std::chrono::seconds>(duration)};
    }
}

std::chrono::duration<double> durationCast(const std::chrono::duration<double>& duration,
                                           time_unit unit) noexcept {
    switch (unit) {
    case time_unit::nanoseconds:
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    case time_unit::microseconds:
        return std::chrono::duration_cast<std::chrono::microseconds>(duration);
    case time_unit::milliseconds:
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    case time_unit::seconds:
        return std::chrono::duration_cast<std::chrono::seconds>(duration);
    case time_unit::minutes:
        return std::chrono::duration_cast<std::chrono::minutes>(duration);
    case time_unit::hours:
        return std::chrono::duration_cast<std::chrono::hours>(duration);
    case time_unit::days:
        return std::chrono::duration_cast<std::chrono::days>(duration);
    case time_unit::weeks:
        return std::chrono::duration_cast<std::chrono::weeks>(duration);
    case time_unit::months:
        return std::chrono::duration_cast<std::chrono::months>(duration);
    case time_unit::years:
        return std::chrono::duration_cast<std::chrono::years>(duration);
    default:
        return std::chrono::duration_cast<std::chrono::seconds>(duration);
    }
};

std::string format(benchtools::time_unit unit) {
    switch (unit) {
        using enum benchtools::time_unit;
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
    return "";
}

std::string format(Duration dur) {
    return std::visit(
        [](auto&& arg) -> std::string {
            using chrono_t = std::decay_t<decltype(arg)>;
            std::string val = std::to_string(arg.count());

            if constexpr (std::is_same_v<chrono_t, std::chrono::nanoseconds>)
                return val + "ns";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::microseconds>)
                return val + "µs";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::milliseconds>)
                return val + "ms";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::seconds>)
                return val + "s";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::minutes>)
                return val + "min";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::hours>)
                return val + "h";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::days>)
                return val + "d";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::weeks>)
                return val + "wk";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::months>)
                return val + "mo";
            else if constexpr (std::is_same_v<chrono_t, std::chrono::years>)
                return val + "y";
            return "";
        },
        dur.duration());
}

[[nodiscard]] std::chrono::zoned_time<std::chrono::duration<double>>
time_date() noexcept {
    std::chrono::zoned_time zoned_time{std::chrono::current_zone(),
                                       std::chrono::system_clock::now()};

    zoned_time = {std::chrono::current_zone(),
                  floor<std::chrono::seconds>(zoned_time.get_local_time())};

    return zoned_time;
};

[[nodiscard]] std::string
format(std::chrono::zoned_time<std::chrono::duration<double>> time_point) noexcept {
    std::ostringstream oss;
    oss << "[" << std::format("{:%F %T}", time_point) << "]" << "" << std::flush;
    return oss.str();
}
}  // namespace benchtools