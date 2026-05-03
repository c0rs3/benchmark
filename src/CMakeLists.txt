# src/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

######## CORE ######## 
add_library(Core
    Core/FileStream.cpp
    Core/Time.cpp
)
target_include_directories(Core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Core PUBLIC cxx_std_20)

######## LOGGER ######## 
add_library(Loggers
    Loggers/FileLogger.cpp
)
target_include_directories(Loggers
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Loggers PUBLIC cxx_std_20)
target_link_libraries(Loggers PUBLIC 
    Core
    spdlog::spdlog)

######## TIMER ######## 
add_library(Timers
    Timers/BaseTimer.cpp
    Timers/ClockTimer.cpp
    Timers/LoggingTimer.cpp
    Timers/ScopedTimer.cpp
    Timers/WallTimer.cpp
)
target_include_directories(Timers
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Timers PUBLIC cxx_std_20)
target_link_libraries(Timers PUBLIC Loggers)


######## TESTING ######## 
option(TESTING "Build test programs" OFF)

if(TESTING)
    add_executable(benchtools_testing
        Test/Test.cpp
    )
    target_link_libraries(benchtools_testing PRIVATE Timers)
    target_compile_features(benchtools_testing PRIVATE cxx_std_20)
endif()