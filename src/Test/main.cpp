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

using namespace benchtools;

int main() {
    benchmark::Profile profile{
        .warmupIterations = 2, .iterations = 20, .policy = benchmark::CPU_Process};
    benchmark::runBenchmark(profile, foo);

    std::clog << std::endl;
    return 0;
}