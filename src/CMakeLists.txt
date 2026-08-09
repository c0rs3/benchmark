# ./src
include(FetchContent)

#spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

# Core
add_library(Core
    Core/Time.cpp
)
target_include_directories(Core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Core PUBLIC cxx_std_20)
target_link_libraries(Core PUBLIC)
add_library(Benchtools::Core ALIAS Core)

# Timer
add_library(Timer
    Timer/Timer.cpp
)
target_include_directories(Timer
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Timer PUBLIC cxx_std_20)
target_link_libraries(Timer PUBLIC Benchtools::Core)
add_library(Benchtools::Timer ALIAS Timer)

# Logger
add_library(Logger
    Logger/Logger.cpp
)
target_include_directories(Logger
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Logger PUBLIC cxx_std_20)
target_link_libraries(Logger PUBLIC Benchtools::Core Benchtools::Timer spdlog::spdlog)
add_library(Benchtools::Logger ALIAS Logger)


# Benchmark
option(TESTING "fucking kys" OFF)

if(TESTING)
    add_executable(benchtools_testing
        Test/main.cpp
    )
    target_link_libraries(benchtools_testing PRIVATE Benchtools::Core Benchtools::Timer Benchtools::Logger)
    target_compile_features(benchtools_testing PRIVATE cxx_std_20)
endif()