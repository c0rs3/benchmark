#include <GLFW/glfw3.h>

#include <string>
#include <string_view>
#include <vector>

namespace benchtools {
namespace plotter {
    class PlotWidget {
      public:
        explicit PlotWidget() noexcept;

        ~PlotWidget() noexcept;

        PlotWidget(const PlotWidget&) = delete;

        PlotWidget& operator=(const PlotWidget&) = delete;

        int Run(int arg_count, char** arg_values) noexcept;

        /**
         * @brief Reads CSV file and loads the data to member variables
         *
         * @param path
         */
        void SetData(std::string_view path);

      private:
        GLFWwindow* m_Window;
        std::vector<double> m_xData, m_yData;
        std::vector<std::string> m_Labels;
        std::string m_Unit;
    };

}  // namespace plotter
}  // namespace benchtools