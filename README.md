# benchmark.h

- A simple benchmarker for code in C++

## Features

- Timer class for benchmarking a scope or a block of code

- A logger class for logging to files

## How to Setup & Use

### CMAKE

- Add this and adjust your CMake accordingly

```CMAKE
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h")

add_executable(benchmarktools main.cpp ... ${SRC_FILES})

target_include_directories(benchmarktools PRIVATE "${CMAKE_SOURCE_DIR}/headers")

target_link_directories(benchmarktools PRIVATE)
target_link_libraries(benchmarktools PRIVATE)
```

- Example usage

```cpp
#include<timer.h>
#include<filelog.h>

using namespace benchtools;

int main() {
    Logger logger("log.txt", std::ios::app);
    logger.log("Starting something...");
    {
        Timer timer1;
        for (size_t i = 0; i < 1000; i++) {
            std::cout << " xd " << std::flush;
        }
    }
    logger.log("Ran successfully!");
    std::clog << last_duration.count() << std::endl;
    logger.log(last_duration.count());
    return 0;
}
```
