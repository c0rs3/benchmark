#include <benchtools/Benchmark/Benchmark.hpp>

#include <benchtools/Plotter/PlotDataLoader.hpp>

#include <iostream>
#include <thread>

void foo() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);
}

int main() {
    using namespace benchtools;
#if 0
    auto cpuTimer = timer::Timer<clock::CPUClock<>>{};

    auto wallTimer = timer::Timer<clock::WallClock>{};
    {
        timer::ScopedTimer scop{cpuTimer};
        cpuTimer.start();
        wallTimer.start();

        std::cin.get();

        cpuTimer.stop();
        wallTimer.stop();
    }
    std::clog << cpuTimer.duration() << std::endl;
    std::clog << time::durationCast(cpuTimer.duration(), time::unit::microseconds)
              << std::endl;
    std::clog << wallTimer.duration();

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
        auto file = timer::FileTimer<clock::WallClock>{"huh.txt"};
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

#elif 1
    benchmark::Profile profile{};
    benchmark::runBenchmark(profile, foo);

    std::clog << std::endl;
    return 0;
#elif 0
    std::filesystem::path path =
        "benchtools_results/[2026-08-11 19:01:33]_benchmarkresults.xml";
    std::clog << Plotter::PlotData::loadData(path.c_str());
#endif
}