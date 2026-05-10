# src/App
add_executable(my_plot_app App.cpp)
target_link_libraries(my_plot_app PRIVATE Timers Plotter)
target_compile_features(my_plot_app PRIVATE cxx_std_20)