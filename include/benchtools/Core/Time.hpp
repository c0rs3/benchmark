#pragma once

#include <chrono>
#include <cstdint>
#include <variant>

namespace benchtools {

struct Duration;

/**
 * @brief expression for default duration
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
    /**
     * @brief Construct a new Duration object from duration_t
     *
     * @param dur
     */
    explicit Duration(duration_t dur) noexcept : dur(dur) {};

  public:
    /**
     * @brief
     *
     * @return std::string
     */
    [[nodiscard]] explicit operator std::string() const noexcept { return format(*this); }

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
    [[nodiscard]] duration_t duration() const noexcept { return dur; }

  private:
    duration_t dur;
};

[[nodiscard]] Duration durationCast(const std::chrono::duration<double>& duration,
                                    time_unit unit) noexcept;

}  // namespace benchtools
