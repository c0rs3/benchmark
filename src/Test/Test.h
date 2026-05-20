
#include <benchtools/File/CSVParser.hpp>
#include <benchtools/File/CSVStream.hpp>

#include <benchtools/Loggers/FileLogger.hpp>
#include <benchtools/Loggers/Logger.hpp>

#include <benchtools/Core/Concepts.hpp>
#include <benchtools/Timers/ClockTimer.hpp>
#include <benchtools/Timers/WallTimer.hpp>
#include <benchtools/Timers/Wrappers/FileTimer.hpp>
#include <benchtools/Timers/Wrappers/LoggingTimer.hpp>
#include <benchtools/Timers/Wrappers/ScopedTimer.hpp>

#include <benchtools/Benchmark/Benchmark.hpp>
#include <benchtools/Benchmark/Policy.hpp>

#include <chrono>
#include <random>
#include <thread>

#include <cstdint>
#include <iostream>
#include <random>

#define ENABLE_TEST_WALL_TIMER 0
#define ENABLE_TEST_CLOCK_TIMER 0
#define ENABLE_TEST_FILE_LOGGER 0
#define ENABLE_TEST_CSV_STREAM 0
#define ENABLE_TEST_CSV_PARSING 0
#define ENABLE_TEST_FILE_TIMER 0
#define ENABLE_TEST_LOG_TIMER 0
#define ENABLE_TEST_LOGGING_TIMER 0
#define ENABLE_TEST_BENCHMARK 0

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace benchtools;
using enum time_unit;

namespace testing {

/**
 * For RNG during testing
 *
 */
inline std::mt19937 engine{};
inline std::uniform_int_distribution<int> dist{1, 100};

/**
 * Callables for benchmark testing
 *
 */
inline void example_callable() {
    static uint32_t counter{0};
    std::cout << "Test: " << ++counter << std::endl;
    std::cin.get();
}

inline void example_callable2() {
    std::this_thread::sleep_for(std::chrono::microseconds(dist(engine)));
}
}  // namespace testing