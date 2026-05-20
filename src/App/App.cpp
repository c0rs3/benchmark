#include <benchtools/Plotting/PlotApp.hpp>

int main(int argc, char** argv) {
    auto& app = PlotApp::GetInstance();
    app.run(argc, argv);
}