
#include "benchtools/Benchmark/Policy.hpp"
#include "benchtools/Core/Time.hpp"
#include "benchtools/File/File.hpp"
#include "benchtools/Loggers/Logger.hpp"
#include <benchtools/Benchmark/Benchmark.hpp>

#include <benchtools/File/CSVParser.hpp>
#include <benchtools/File/CSVStream.hpp>

#include <benchtools/Loggers/FileLogger.hpp>

#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/FileTimer.hpp>
#include <benchtools/Timers/LoggingTimer.hpp>
#include <benchtools/Timers/ScopedTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>
#include <chrono>
#include <random>
#include <thread>

#include <cstdint>
#include <iostream>
#include <random>

std::mt19937 engine{};
std::uniform_int_distribution<int> a{1, 100};

using namespace std::string_literals;
using namespace std::chrono_literals;

void example_callable() {
    static uint32_t counter{0};
    std::cout << "Test: " << ++counter << std::endl;
    std::cin.get();
}

void example_callable2() {
    std::this_thread::sleep_for(std::chrono::microseconds(a(engine)));
}

int main() {
    using namespace benchtools;
    using enum time_unit;
#if 0
    WallTimer timer{};
    {
        ScopedTimer scop(timer);
        std::cin.get();
    }
    std::clog << timer.duration(nanoseconds).str() << std::endl;
    std::clog << timer.duration(microseconds).str() << std::endl;
    std::clog << timer.duration(milliseconds).str() << std::endl;
    std::clog << timer.duration(seconds).str() << std::endl;
    std::clog << timer.duration(minutes).str() << std::endl;
    std::clog << timer.duration(hours).str() << std::endl;
    std::clog << timer.duration(days).str() << std::endl;
    std::clog << timer.duration(weeks).str() << std::endl;
    std::clog << timer.duration(months).str() << std::endl;
    std::clog << timer.duration(years).str() << std::endl;
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
#elif 0
    ClockTimer timer{};
    {
        ScopedTimer tim{timer};
        std::cin.get();
    }
    std::clog << timer.duration(nanoseconds).str() << std::endl;
    std::clog << timer.duration(microseconds).str() << std::endl;
    std::clog << timer.duration(milliseconds).str() << std::endl;
    std::clog << timer.duration(seconds).str() << std::endl;
    std::clog << timer.duration(minutes).str() << std::endl;
    std::clog << timer.duration(hours).str() << std::endl;
    std::clog << timer.duration(days).str() << std::endl;
    std::clog << timer.duration(weeks).str() << std::endl;
    std::clog << timer.duration(months).str() << std::endl;
    std::clog << timer.duration(years).str() << std::endl;
    {
        ScopedTimer tim{timer};
        std::cin.get();
    }
    std::clog << timer.duration(nanoseconds).str() << std::endl;
    std::clog << timer.duration(microseconds).str() << std::endl;
    std::clog << timer.duration(milliseconds).str() << std::endl;
    std::clog << timer.duration(seconds).str() << std::endl;
    std::clog << timer.duration(minutes).str() << std::endl;
    std::clog << timer.duration(hours).str() << std::endl;
    std::clog << timer.duration(days).str() << std::endl;
    std::clog << timer.duration(weeks).str() << std::endl;
    std::clog << timer.duration(months).str() << std::endl;
    std::clog << timer.duration(years).str() << std::endl;
#elif 0

    FileLogger a{"ig.txt"};
    for (auto x{0}; x < 25; x++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        a.Log("Something", LogType(x % 5));
    }
#elif 0
    std::array<std::string_view, 3> a = {"1", "tuna", "example@gmail.com"};
    CSVStream<3> stream{"text.csv", "id", "name", "email"};
    // stream.write("1", "tuna", "example@gmail.com");
    stream.write(a);
#elif 1
    ClockTimer timer;
    FileTimer<ClockTimer> tim{timer, "log.txt", time_unit::nanoseconds, fileopen::append};
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
#elif 0
    std::clog << benchmark<Policy::Wall>(example_callable, 2);
    TRACE(std::to_string(benchmark<Policy::CPU>(example_callable, 2)));
#elif 0
    CSVParser<3> p{"result.csv"};
#elif 1
    std::clog << benchmark<Policy::Wall>(example_callable2, 1000, time_unit::nanoseconds);
#endif
    std::clog << std::endl;
    return 0;
}