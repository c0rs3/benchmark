#pragma once

#include <cassert>
#include <cstdint>
#include <string>

namespace benchtools {

namespace log {

    enum class type : uint8_t {
        TIMER,
        INFO,
        TRACE,
        WARNING,
        ERROR,
    };

    [[nodiscard]] inline std::string format(log::type type) {
        switch (type) {
            using enum benchtools::log::type;
        case TIMER:
            return "[TIME]";
        case INFO:
            return "[INFO]";
        case TRACE:
            return "[TRACE]";
        case WARNING:
            return "[WARN]";
        case ERROR:
            return "[ERROR]";
        default:
            assert("Unknown" && 0);
            return "[UNKNOWN]";
        }
    }
};  // namespace log
}  // namespace benchtools