# src/App
add_executable(my_plot_app Plot.cpp)
target_link_libraries(my_plot_app PRIVATE Timers implot imgui glfw)
target_compile_features(my_plot_app PRIVATE cxx_std_20)