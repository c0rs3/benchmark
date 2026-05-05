#pragma once

#include <concepts>
#include <cstddef>
#include <string>
#include <type_traits>

namespace benchtools {

class ClockTimer;
class WallTimer;
class FileLogger;
template <size_t N>
class CSVStream;

template <typename T>
concept convertible_to_string =
    std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

template <typename T>
concept clock_or_wall_timer =
    std::is_same_v<T, ClockTimer> || std::is_same_v<T, WallTimer>;

template <typename T>
concept is_policy = clock_or_wall_timer<T>;

template <typename T, size_t N>
concept text_or_csv_stream = std::is_same_v<T, size_t> || std::is_same_v<T, size_t>;
};  // namespace benchtools