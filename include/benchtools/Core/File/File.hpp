#pragma once

#include <filesystem>
#include <ios>

namespace benchtools {

namespace file {
    using fmode_t = std::ios_base::openmode;

    namespace fileopen {
        constexpr inline fmode_t append = std::ios::app | std::ios::out;
        constexpr inline fmode_t insert = std::ios::trunc | std::ios::out;
        constexpr inline fmode_t input = std::ios::in;
    };  // namespace fileopen

    using File = std::filesystem::path;

}  // namespace file
}  // namespace benchtools