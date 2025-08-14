#include <timer.h>
namespace benchtools {
    Timer::Timer() {
        mStart = std::chrono::steady_clock::now();
    }

    Timer::Timer(timeunits unit = timeunits::nanosecond){
        mStart = std::chrono::steady_clock::now();
        mUnit = unit;
    };
    
    Timer::~Timer() {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        mDuration = end - mStart;
        last_duration = mDuration;
        switch (mUnit) {
        case timeunits::nanosecond:
            std::clog << "Duration(ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(mDuration).count() << "ns" << std::endl;
            break;
        case timeunits::microsecond:
            std::clog << "Duration(μs): " << std::chrono::duration_cast<std::chrono::microseconds>(mDuration).count() << "μs" << std::endl;
            break;
        case timeunits::milisecond:
            std::clog << "Duration(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(mDuration).count() << "ms" << std::endl;
            break;
        case timeunits::second:
            std::clog << "Duration(seconds): " << std::chrono::duration_cast<std::chrono::seconds>(mDuration).count() << "s" << std::endl;
            break;
        case timeunits::minute:
            std::clog << "Duration(minutes): " << std::chrono::duration_cast<std::chrono::seconds>(mDuration).count() << "m" << std::endl;
            break;
        case timeunits::hour:
            std::clog << "Duration(hours): " << std::chrono::duration_cast<std::chrono::hours>(mDuration).count() << "h" << std::endl;
            break;
        case timeunits::Default:
            std::clog << "Duration(ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(mDuration).count() << "ns" << std::endl;
            std::clog << "Duration(ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(mDuration).count() << "ms" << std::endl;
            break;
        default:
            break;
        }
    }
}