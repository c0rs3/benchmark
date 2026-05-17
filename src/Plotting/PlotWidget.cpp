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
        // ImGui_ImplOpenGL3_Shutdown();  // Fixed? this was giving a fucking segfault
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

        for (const auto& str : m_Labels)
            label_view.push_back(str.c_str());

        std::string plotTitle = "Time Durations (" + m_TimerType + ")";

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            glfwGetFramebufferSize(m_Window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Make the ImGui window cover the whole GLFW client area
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            // Begin with no flags (or add NoMove | NoResize if you want to lock it)
            ImGui::Begin(arg_values[1], nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

            // Plot fills all available space after the title bar
            ImVec2 plotSize = ImGui::GetContentRegionAvail();

            if (ImPlot::BeginPlot(plotTitle.c_str(), plotSize, ImPlotFlags_None)) {

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

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_Window);
        }
        return 0;
    }
}  // namespace plotter
}  // namespace benchtools