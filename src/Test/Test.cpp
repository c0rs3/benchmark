#if 1
    #include <benchtools/Benchmark/Benchmark.hpp>

    #include <benchtools/File/CSVParser.hpp>
    #include <benchtools/File/CSVStream.hpp>

    #include <benchtools/Loggers/FileLogger.hpp>

    #include <benchtools/Timers.hpp>

using namespace benchtools;
using enum time_unit;

    #include <cstdint>
    #include <iostream>

using namespace std::string_literals;
using namespace std::chrono_literals;

void example_callable() {
    static uint32_t counter{0};
    std::cout << "Test: " << ++counter << std::endl;
    std::cin.get();
}

int main() {
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
    #elif 0
    WallTimer timer;
    /**
     * @brief Retarded time unit setting does not work at all
     *
     */
    FileTimer<WallTimer> tim{timer, "ig.txt", time_unit::seconds};
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
    #elif 0
    CSVParser<3> p{"result.csv"};
    #endif
    return 0;
}

#endif