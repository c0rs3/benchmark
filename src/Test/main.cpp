
#include "benchtools/Core/Core.hpp"
#include "benchtools/Core/Time.hpp"
#include <benchtools/Benchmark/Benchmark.hpp>

#include <benchtools/Core/Benchmark/Policy.hpp>
#include <benchtools/Core/Clocks/CPUClock.hpp>

#include <benchtools/Logger/Timer/LoggingTimer.hpp>

#include <benchtools/Timer/FileTimer.hpp>
#include <benchtools/Timer/ScopedTimer.hpp>
#include <benchtools/Timer/Timer.hpp>

#include <iostream>
#include <thread>

void foo() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);
}

int main() {
    using namespace benchtools;
    using namespace benchtools::clock;
    using namespace benchtools::timer;
    auto processTimer = Timer<CPUClock<>>{};
    ScopedTimer tim{processTimer};    
    {
        auto log = timer::LoggingTimer<clock::WallClock>{};
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
        auto file = timer::FileTimer<clock::WallClock>{"example.txt"};
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

    benchmark::Profile profile{
        .warmupIterations = 2, .iterations = 20, .policy = benchmark::CPU_Process};
    benchmark::runBenchmark(profile, foo);

    std::clog << std::endl;
    return 0;
}