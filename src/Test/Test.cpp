#include "Test.h"

int main() {

#if ENABLE_TEST_WALL_TIMER
    {
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
    }
#endif

#if ENABLE_TEST_CLOCK_TIMER
    {
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
    }
#endif

#if ENABLE_TEST_FILE_LOGGER
    {
        FileLogger a{"filelog.txt"};
        for (auto x{0}; x < 25; x++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            a.Log("Something", LogType(x % 5));
        }
    }
#endif

#if ENABLE_TEST_CSV_STREAM
    {
        std::array<std::string_view, 3> a = {"1", "tuna", "example@gmail.com"};
        CSVStream<3> stream{"text.csv", fileopen::append, "id", "name", "email"};
        stream.write(a);
    }
#endif

#if ENABLE_TEST_CSV_PARSING
    {
        auto headers = CSVParser<3>::getHeaders("text.csv");
        auto rows = CSVParser<3>::getRows("text.csv");
    }
#endif

#if ENABLE_TEST_FILE_TIMER
    {
        ClockTimer timer;
        FileTimer tim{timer, "log.txt", fileopen::append, time_unit::nanoseconds};
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
    }
#endif

#if ENABLE_TEST_LOGGING_TIMER
    {
        WallTimer timer;
        LoggingTimer tim{timer, time_unit::nanoseconds};
        {
            tim.start();
            // std::cin.get();
            tim.stop();
        }
        {
            tim.start();
            // std::cin.get();
            tim.stop();
        }
        {
            tim.start();
            // std::cin.get();
            tim.stop();
        }
        {
            tim.start();
            // std::cin.get();
            tim.stop();
        }
    }
#endif

#if ENABLE_TEST_BENCHMARK
    {
        auto average_dur =
            benchmark<Policy::System>(10, time_unit::microseconds, example_callable2);
    }
#endif
    BENCHTOOLS_CRITICAL("{}", 1);
    return 0;
}