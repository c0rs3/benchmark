#include <benchtools/Plotting/Widgets/PlotWidget.hpp>

class PlotApp {
  public:
    PlotApp(const PlotApp&) = delete;
    PlotApp& operator=(const PlotApp&) = delete;
    PlotApp(PlotApp&&) = delete;
    PlotApp& operator=(PlotApp&&) = delete;

    static benchtools::plotter::PlotWidget& GetInstance() {
        static PlotApp instance{};
        return instance.Interface();
    }

    benchtools::plotter::PlotWidget& Interface() { return mWidget; }

  private:
    PlotApp(std::string_view name = "PlotApp") : mWidget() {}

  private:
    benchtools::plotter::PlotWidget mWidget;
};