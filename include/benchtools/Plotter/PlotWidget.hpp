#pragma once

#include <benchtools/Core/Benchmark/Result.hpp>

#include <string_view>

class GLFWwindow;

namespace benchtools::plotter {

/** @brief Plotting App Singleton */
class PlotWidget {
  public:
    ~PlotWidget() noexcept(0);

    PlotWidget(const PlotWidget&) = delete;
    PlotWidget& operator=(const PlotWidget&) = delete;
    PlotWidget(PlotWidget&&) noexcept = delete;
    PlotWidget& operator=(PlotWidget&&) noexcept = delete;

    static PlotWidget& getInstance() noexcept;

    void run(int arg_count, char** arg_values);

  private:
    PlotWidget() = default;
    void setData(std::string_view path);

  private:
    GLFWwindow* m_Window;
    benchmark::Result m_Data;
};
}  // namespace benchtools::plotter