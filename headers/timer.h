#ifndef BENCHMARK_TIMER
#define BENCHMARK_TIMER
#include <iostream>
#include <chrono>
namespace benchtools {
    enum timeunits {
        nanosecond = 0x3B9ACA00,
        microsecond = 0x000003E8,
        milisecond = 0x000F4240,
        second = 0x00000000,
        Default = 0x00000001,
        minute = 0x0000003C,
        hour = 0x00000E10,
    };

    // After each scope use this variable to get the duration of that scope
    static std::chrono::duration<double> last_duration;

    class Timer {
    private:
        std::chrono::duration<double> mDuration;
        std::chrono::steady_clock::time_point mStart;
        timeunits mUnit = Default;
    public:
        Timer();

        Timer(timeunits unit);

        ~Timer();
    };
}
#endif
