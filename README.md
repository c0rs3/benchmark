# benchmark.h

- Simple code benchmarking tools for C++

## Features

- Timer class for benchmarking a scope or a block of code

- A logger class for logging to files

## How to Setup

- Requirements
  - C++14 or higher

### CMake (MSVC/GNU)

- Add this and adjust your CMake accordingly

```CMAKE
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
# Glob the source files however you want
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h")
# Glob the header files however you want

add_executable(benchmarktools main.cpp ... ${SRC_FILES})
# Add them as source files

target_include_directories(benchmarktools PRIVATE "${CMAKE_SOURCE_DIR}/headers")

target_link_directories(benchmarktools PRIVATE)
target_link_libraries(benchmarktools PRIVATE)
```

### Visual Studio (.sln)

- Add the source and header files to your project

### GNU (standalone)

- Add the source and header files to the ```g++``` arguments

```bash
g++ main.cpp benchtools/src/filelog.cpp benchtools/src/timer.cpp -I benchtools/headers
```

## How to Use

- Example usage

```cpp
#include <benchmark.h> // Include all the utilities
```

```cpp
using namespace benchtools;

int main() {

    // Initialize the file logger
    Logger logger("log.txt", std::ios::app); 
    logger.log("Starting something...");

    // The scope to be benchmarked
    {
        Timer timer1;
        for (size_t i = 0; i < 1000; i++) {
            std::cout << " LOG " << std::flush;
        }
    }
    logger.log("Ran successfully!");

    // Use LAST_DURATION to retrieve the last duration recorded
    std::clog << LAST_DURATION.count() << std::endl;
    logger.log(LAST_DURATION.count());
    return 0;

}
```

- Example usage with ```#define EXPLICIT_TIMER```

```cpp
#define EXPLICIT_TIMER // Timer destructor is now explicit

using namespace benchtools;

int main() {

    // Initialize the file logger
    Logger logger("log.txt", std::ios::app); 
    logger.log("Starting something...");

    // The scope to be benchmarked
    Timer timer1;
    for (size_t i = 0; i < 1000; i++) {
        std::cout << " LOG " << std::flush;
    }
    timer1.~Timer();
    logger.log("Ran successfully!");

    // Use LAST_DURATION to retrieve the last duration recorded
    std::clog << LAST_DURATION.count() << std::endl;
    logger.log(LAST_DURATION.count());

    return 0;

}
```
