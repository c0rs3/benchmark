#include <benchtools/Plotting/Data/DataLoader.hpp>
#include <benchtools/Plotting/Widgets/PlotWidget.hpp>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <algorithm>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

namespace benchtools {
namespace plotter {
    PlotWidget::PlotWidget() noexcept {}

    PlotWidget::~PlotWidget() noexcept {
        glfwMakeContextCurrent(m_Window);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void PlotWidget::SetData(std::string_view path) {
        auto&& [xData, yData, labels, unit, timer_t] =
            benchtools::plotter::DataLoader::LoadFromCSV(path);

        m_xData = std::move(xData);
        m_yData = std::move(yData);
        m_Labels = std::move(labels);
        m_Unit = std::move(unit);
        m_TimerType = std::move(timer_t);
    }

    int PlotWidget::Run(int arg_count, char** arg_values) noexcept {
        if (arg_count < 2) {
            std::cerr << "Usage" << arg_values[0] << "<csv_file>\n";
            return 1;
        }
        SetData(arg_values[1]);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, arg_values[1], nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        glfwMakeContextCurrent(m_Window);

        int display_w{}, display_h{};

        std::vector<const char*> label_view;
        label_view.reserve(m_Labels.size());

        std::transform(m_Labels.begin(), m_Labels.end(), label_view.begin(),
                       [](std::string& a) { return a.c_str(); });

        std::string plotTitle = "Time Durations (" + m_TimerType + ")";

        ImVec2 lastWindowSize(0, 0);
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            int display_w, display_h;
            glfwGetFramebufferSize(m_Window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin(arg_values[1], nullptr, ImGuiWindowFlags_AlwaysAutoResize);

            if (ImPlot::BeginPlot(
                    plotTitle.c_str(),
                    ImVec2(m_xData.size() * 8 > 1920 ? 1920 : m_xData.size() * 8 > 1920,
                           0))) {
                ImPlot::SetupAxis(ImAxis_X1, "Timer ID", ImPlotAxisFlags_LockMin);
                std::string yLabel = "Duration (" + m_Unit + ")";
                ImPlot::SetupAxis(ImAxis_Y1, yLabel.c_str(), ImPlotAxisFlags_LockMin);

                ImPlot::SetupAxisTicks(ImAxis_X1, m_xData.data(), (int)m_yData.size(),
                                       label_view.data());

                double maxY = *std::max_element(m_yData.begin(), m_yData.end());
                ImPlot::SetupAxesLimits(-1, m_xData.back() + 1, 0, maxY * 1.1);

                ImPlot::PlotBars("Duration", m_xData.data(), m_yData.data(),
                                 (int)m_yData.size(), 0.6);

                ImPlot::EndPlot();
            }

            ImVec2 windowSize = ImGui::GetWindowSize();
            ImGui::End();

            const float decorationPadding = 10.0f;
            int newWidth = static_cast<int>(windowSize.x + decorationPadding);
            int newHeight = static_cast<int>(windowSize.y + decorationPadding);

            if (newWidth != lastWindowSize.x || newHeight != lastWindowSize.y) {
                glfwSetWindowSize(m_Window, newWidth, newHeight);
                lastWindowSize = ImVec2((float)newWidth, (float)newHeight);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_Window);
        }
        return 0;
    }
}  // namespace plotter
}  // namespace benchtools