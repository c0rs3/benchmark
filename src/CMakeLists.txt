# ./src
include(FetchContent)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(spdlog)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.0
)
FetchContent_MakeAvailable(imgui)

# GLFW
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

# ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG        v1.0
)
FetchContent_MakeAvailable(implot)

# Link implot with imgui
add_library(implot STATIC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Cross-platform OpenGL linkage
if(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui PUBLIC OpenGL::GL)
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARY})
elseif(WIN32)
    target_link_libraries(imgui PUBLIC opengl32)
endif()

# Core
add_library(Core
    Core/Time.cpp
    Core/Result.cpp
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
add_library(Benchmark
    Benchmark/Benchmark.cpp
)
target_include_directories(Benchmark
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Benchmark PUBLIC cxx_std_20)
target_link_libraries(Benchmark PUBLIC Benchtools::Core Benchtools::Timer)
add_library(Benchtools::Benchmark ALIAS Benchmark)

# Plotter
add_library(Plotter
    Plotter/PlotWidget.cpp
)
target_include_directories(Plotter
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_compile_features(Plotter PUBLIC cxx_std_20)
target_link_libraries(Plotter PUBLIC Benchtools::Core implot imgui glfw)
add_library(Benchtools::Plotter ALIAS Plotter)

option(TESTING OFF)

if(TESTING)
    add_executable(benchtools_testing
        Test/main.cpp
    )
    target_link_libraries(benchtools_testing PRIVATE Benchtools::Core Benchtools::Timer Benchtools::Logger)
    target_compile_features(benchtools_testing PRIVATE cxx_std_20)
endif()

if(NOT(TESTING))
    add_subdirectory(App)
endif()
