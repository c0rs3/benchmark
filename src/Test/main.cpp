#include "benchtools/Core/Clocks/CPUClock.hpp"
#include "benchtools/Core/Clocks/WallClock.hpp"
#include "benchtools/Core/Time.hpp"

#include "benchtools/Logger/Timer/LoggingTimer.hpp"

#include "benchtools/Timer/FileTimer.hpp"
#include "benchtools/Timer/ScopedTimer.hpp"
#include <benchtools/Timer/Timer.hpp>

#include <benchtools/Benchmark/Benchmark.hpp>

#include <iostream>
#include <thread>

void foo() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);
}

int main() {
#if 0
    auto cpuTimer = benchtools::timer::Timer<
    benchtools::clock::CPUClock<benchtools::clock::ClockType::Process>>{};

    auto wallTimer = benchtools::timer::Timer<benchtools::clock::WallClock>{};
    {
        benchtools::timer::ScopedTimer scop{cpuTimer};
        cpuTimer.start();
        wallTimer.start();

        std::cin.get();

        cpuTimer.stop();
        wallTimer.stop();
    }

    {
        auto log = benchtools::timer::LoggingTimer<benchtools::clock::WallClock>{};
        log.start();
        std::cin.get();
        log.stop();

        log.start();
        std::cin.get();
        log.stop();

        log.start(1);
        std::cin.get();
        log.stop();
    }

    {
        auto file =
        benchtools::timer::FileTimer<benchtools::clock::WallClock>{"huh`.txt"};
        file.start();
        std::cin.get();
        file.stop();

        file.start();
        std::cin.get();
        file.stop();

        file.start(1);
        std::cin.get();
        file.stop();
    }

    std::clog << cpuTimer.duration() << std::endl;
    std::clog << benchtools::time::durationCast(cpuTimer.duration(),
    benchtools::time::unit::microseconds)
    << std::endl;
    std::clog << wallTimer.duration();
#endif
    benchtools::benchmark::Profile profile{};
    benchtools::benchmark::benchmarkFunc(profile, foo);
    std::clog << std::endl;
    return 0;
}