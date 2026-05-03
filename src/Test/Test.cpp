#include "benchtools/Timers/WallTimer.hpp"
#include <benchtools/Core/CSVStream.hpp>
#include <benchtools/Loggers/FileLogger.hpp>
#include <benchtools/Timers.hpp>

#include <benchtools/Timers/FileTimer.hpp>
#include <iostream>
#include <thread>

using namespace benchtools;
using enum time_unit;

using namespace std::string_literals;

int main() {
#if 1
    WallTimer timer{};
    {
        ScopedTimer scop(timer);
        std::cin.get();
    }
    std::clog << timer.duration(seconds).str() << std::endl;
    {
        ScopedTimer scop(timer);
        std::cin.get();
    }
    std::clog << timer.duration(seconds).str() << std::endl;
    {
        ScopedTimer scop(timer);
        std::cin.get();
    }
    std::clog << timer.duration(seconds).str() << std::endl;
    timer.reset();
#elif 0
    ClockTimer timer{};
    {
        LoggingTimer login{timer, nanoseconds};
        std::cin.get();
    }
    {
        LoggingTimer login{timer, nanoseconds};
        std::cin.get();
        login.stop();
    }
#elif 0
    FileLogger a{"ig.txt"};
    for (auto x{0}; x < 25; x++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        a.Log("Something", LogType(x % 5));
    }
#elif 0
    std::array<std::string_view, 3> a = {"1", "tuna", "example@gmail.com"};
    CSVStream<3> stream{"text.csv", "id", "name", "email"};
    // stream.write("1", "tuna", "example@gmail.com");
    stream.write(a);
#elif 0
    WallTimer timer;
    FileTimer<WallTimer> tim{timer, "ig.txt"};
    {
        tim.start();
        std::cin.get();
        tim.stop();
    }
    {
        tim.start();
        std::cin.get();
        tim.stop();
    }
    {
        tim.start();
        std::cin.get();
        tim.stop();
    }
    {
        tim.start();
        std::cin.get();
        tim.stop();
    }
#endif
}