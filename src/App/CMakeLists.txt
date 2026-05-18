# src/App
add_executable(plot_app App.cpp)

set_target_properties(plot_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(plot_app PRIVATE 
    Timers 
    Plotter
    imgui
    glfw   
)

target_compile_features(plot_app PRIVATE cxx_std_20)