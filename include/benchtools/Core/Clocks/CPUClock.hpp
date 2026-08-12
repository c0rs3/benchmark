#pragma once

#include <benchtools/Core/Platform.hpp>

#include <chrono>

#ifdef BENCHTOOLS_LIN
    #include <time.h>
#elif BENCHTOOLS_WIN
    #include <windows.h>
#endif

namespace benchtools {
namespace clock {

    enum class CPU {
        Process,  // PROCESS CLOCK
        Thread    // THREAD CLOCK
    };

    /**
     * @brief CPUClock for chrono
     * @tparam Type either thread or process clock
     */
    template <CPU Type = CPU::Process>
    class CPUClock {
      public:
        using duration = std::chrono::nanoseconds;
        using rep = duration::rep;
        using period = duration::period;
        using time_point = std::chrono::time_point<CPUClock>;

        /**
         * @brief Returns the current time_point
         * @note Only for WIN and Linux systems
         * @return time_point
         */
        [[nodiscard]] static time_point now() noexcept {
#if defined(BENCHTOOLS_LIN)
            struct timespec ts;
            if constexpr (Type == CPU::Process) {
                if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts)) return {};
            } else if (Type == CPU::Thread) {
                if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts)) return {};
            }

            return time_point(std::chrono::seconds(ts.tv_sec) +
                              std::chrono::nanoseconds(ts.tv_nsec));
#elif defined(BENCHTOOLS_WIN)
            FILETIME creation, exit, kernel, user;
            BOOL ok;

            if constexpr (Type == ClockType::Process)
                ok = GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel,
                                     &user);
            else
                ok = GetThreadTimes(GetCurrentThread(), &creation, &exit, &kernel, &user);

            if (!ok) return {};

            auto filetime_to_ns = [](const FILETIME& ft) -> long long {
                ULARGE_INTEGER u;
                u.LowPart = ft.dwLowDateTime;
                u.HighPart = ft.dwHighDateTime;
                return static_cast<long long>(u.QuadPart) * 100LL;  // 100ns -> ns
            };

            long long total_ns = filetime_to_ns(kernel) + filetime_to_ns(user);
            return time_point(std::chrono::nanoseconds(total_ns));
#else
            return {};
#endif
        }
    };

}  // namespace clock
}  // namespace benchtools