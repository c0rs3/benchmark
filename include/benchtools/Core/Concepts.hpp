#pragma once

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>

#include <concepts>
#include <string>
#include <type_traits>

namespace benchtools {

template <class T>
concept convertible_to_string =
    std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

template <class T>
concept clock_or_wall_timer =
    std::is_same_v<T, timers::ClockTimer> || std::is_same_v<T, timers::WallTimer>;
};  // namespace benchtools