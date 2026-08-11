#include <benchtools/Plotter/PlotWidget.hpp>
#include <exception>
#include <iostream>

int main(int argc, char** argv) {
    auto& instance = benchtools::plotter::PlotWidget::getInstance();
    try {
        instance.run(argc, argv);
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
}