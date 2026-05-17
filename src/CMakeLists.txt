include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# Dear ImGui (required by ImPlot)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.0
)
FetchContent_MakeAvailable(imgui)

# GLFW (for the ImGui GLFW backend) – must be fetched before imgui uses it
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw)


# ImPlot v1.0
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG        v1.0
)
FetchContent_MakeAvailable(implot)

add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Platform OpenGL linkage (required by the OpenGL3 backend)
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui PUBLIC OpenGL::GL)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARY})
elseif(WIN32)
    target_link_libraries(imgui PUBLIC opengl32)
endif()


######## CORE ######## 
add_library(Core
    Core/Time.cpp
)
target_include_directories(Core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Core PUBLIC cxx_std_20)
add_library(Benchtools::Core ALIAS Core)

######## File ######## 
add_library(File
    File/FileStream.cpp
)
target_include_directories(File
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(File PUBLIC cxx_std_20)
add_library(Benchtools::File ALIAS File)

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
    Benchtools::Core
    Benchtools::File
    spdlog::spdlog)
add_library(Benchtools::Loggers ALIAS Loggers)

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
target_link_libraries(Timers PUBLIC Benchtools::Loggers)
add_library(Benchtools::Timers ALIAS Timers)

######## Benchmark ######## 
add_library(Benchmark
    Benchmark/Benchmark.cpp
)
target_include_directories(Benchmark
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Benchmark PUBLIC cxx_std_20)
target_link_libraries(Benchmark PUBLIC Benchtools::Timers Benchtools::Core Benchtools::File)
add_library(Benchtools::Benchmark ALIAS Benchmark)

######## PLOTTING ######## 

# find_package(TBB REQUIRED COMPONENTS tbb)

add_library(Plotter
    Plotting/PlotWidget.cpp
    Plotting/DataLoader.cpp
)

target_include_directories(Plotter PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_compile_features(Plotter PUBLIC cxx_std_20)
target_link_libraries(Plotter PUBLIC Benchtools::File Benchtools::Core implot imgui glfw)
add_library(Benchtools::Plotter ALIAS Plotter)

######## TESTING ######## 
option(BENCHTOOLS_TESTING "Dev testing" OFF)

if(BENCHTOOLS_TESTING)
    add_executable(benchtools_testing
        Test/Test.cpp
    )
    target_link_libraries(benchtools_testing PRIVATE Benchtools::Timers)
    target_compile_features(benchtools_testing PRIVATE cxx_std_20)
endif()

option(BENCHTOOLS_PLOT "Plotting app" OFF)

if(BENCHTOOLS_PLOT)
    add_subdirectory(App)
endif()