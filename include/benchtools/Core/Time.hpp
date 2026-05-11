#pragma once

#include <chrono>
#include <cstdint>
#include <type_traits>
#include <variant>

namespace benchtools {

struct Duration;

/**
 * @brief default expression for default durations
 *
 */
inline constexpr auto default_duration = std::chrono::duration<double>::zero();

/**
 * @brief enum for time units
 *
 */
enum class time_unit : uint8_t {
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

#ifndef UNIT_MICROS
    #define UNIT_MICROS "µs"
#endif

/**
 * @brief type interface for time_unit -> chrono
 *
 * @tparam Unit
 */
template <time_unit Unit>
struct chrono_duration_type;

template <>
struct chrono_duration_type<time_unit::nanoseconds> {
    using type = std::chrono::nanoseconds;
};
template <>
struct chrono_duration_type<time_unit::microseconds> {
    using type = std::chrono::microseconds;
};
template <>
struct chrono_duration_type<time_unit::milliseconds> {
    using type = std::chrono::milliseconds;
};
template <>
struct chrono_duration_type<time_unit::seconds> {
    using type = std::chrono::seconds;
};
template <>
struct chrono_duration_type<time_unit::minutes> {
    using type = std::chrono::minutes;
};
template <>
struct chrono_duration_type<time_unit::hours> {
    using type = std::chrono::hours;
};
template <>
struct chrono_duration_type<time_unit::days> {
    using type = std::chrono::days;
};
template <>
struct chrono_duration_type<time_unit::weeks> {
    using type = std::chrono::weeks;
};
template <>
struct chrono_duration_type<time_unit::months> {
    using type = std::chrono::months;
};
template <>
struct chrono_duration_type<time_unit::years> {
    using type = std::chrono::years;
};

/**
 * @brief returns the unit enum as a string
 *
 * @param unit
 * @return std::string
 */
[[nodiscard]] std::string format(benchtools::time_unit unit);

/**
 * @brief returns the duration as {count} {unit}
 *
 * @param dur
 * @return std::string
 */
[[nodiscard]] std::string format(Duration dur);

/**
 * @brief
 * @brief
 *
 * @param time_point
 * @return std::string
 */
[[nodiscard]] std::string
format(std::chrono::zoned_time<std::chrono::duration<double>> time_point) noexcept;

/**
 * @brief
 *
 * @return std::chrono::zoned_time<std::chrono::duration<double>>
 */
[[nodiscard]] std::chrono::zoned_time<std::chrono::duration<double>> time_date() noexcept;

/**
 * @brief Get the Duration object
 *
 * @param duration
 * @param unit
 * @return Duration
 */
[[nodiscard]] Duration getDuration(const std::chrono::duration<double>& duration,
                                   time_unit unit) noexcept;
/**
 * @brief
 *
 * @param duration
 * @param unit
 * @return std::chrono::duration<double>
 */
[[nodiscard]] std::chrono::duration<double>
durationCast(const std::chrono::duration<double>& duration, time_unit unit) noexcept;

/**
 * @brief
 *
 * @tparam T
 */
template <typename T>
    requires(std::is_same_v<T, float> || std::is_same_v<T, double>)
[[nodiscard]] time_unit get_unit(std::chrono::duration<T> dur) {
    T abs_value = dur.count();

    if (abs_value >= 1.0) {
        return time_unit::seconds;
    } else if (abs_value >= 1e-3) {
        return time_unit::milliseconds;
    } else if (abs_value >= 1e-6) {
        return time_unit::microseconds;
    } else {
        return time_unit::nanoseconds;
    }
};

/**
 * @brief Duration wrapper for the duration_t variant
 *
 */
struct Duration {
    /**
     * @brief alias for std::variant<chrono types>
     *
     */
    using duration_t =
        std::variant<std::chrono::nanoseconds, std::chrono::microseconds,
                     std::chrono::milliseconds, std::chrono::seconds,
                     std::chrono::minutes, std::chrono::hours, std::chrono::days,
                     std::chrono::weeks, std::chrono::months, std::chrono::years>;

  public:
    explicit Duration() noexcept {
        m_Duration = getDuration(default_duration, time_unit::seconds).duration();
    };
    /**
     * @brief Construct a new Duration object from duration_t
     *
     * @param dur
     */
    explicit Duration(duration_t dur) noexcept : m_Duration(dur) {};

  public:
    /**
     * @brief
     *
     * @return std::string
     */
    [[nodiscard]] std::string str() const noexcept { return format(*this); }

    /**
     * @brief
     *
     * @return duration_t
     */
    [[nodiscard]] duration_t duration() const noexcept { return m_Duration; }

    /**
     * @brief
     *
     * @return std::string
     */
    [[nodiscard]] explicit operator std::string() const noexcept { return format(*this); }

    /**
     * @brief
     *
     * @param other
     * @return Duration
     */
    [[nodiscard]] Duration operator+(const Duration& other) const noexcept {
        return Duration{getDuration(this->get_chrono_t() + other.get_chrono_t(),
                                    time_unit::nanoseconds)};
    }

    /**
     * @brief
     *
     * @param val
     * @return Duration
     */
    [[nodiscard]] Duration operator/(int val) const noexcept {
        return Duration{getDuration(this->get_chrono_t() / val, time_unit::nanoseconds)};
    }

    /**
     * @brief Get the chrono t object
     *
     * @return std::chrono::duration<double>
     */
    [[nodiscard]] std::chrono::duration<double> get_chrono_t() const noexcept {
        return std::visit(
            [](auto&& arg) -> std::chrono::duration<double> {
                using chrono_t = std::decay_t<decltype(arg)>;

                return std::chrono::duration_cast<chrono_t>(arg);
            },
            m_Duration);
    }

  private:
    duration_t m_Duration;
};

}  // namespace benchtools
