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

template <class T>
concept convertible_to_string =
    std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

template <class T>
concept clock_or_wall_timer =
    std::is_same_v<T, ClockTimer> || std::is_same_v<T, WallTimer>;
};  // namespace benchtools