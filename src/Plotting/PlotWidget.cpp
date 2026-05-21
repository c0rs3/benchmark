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

    void PlotWidget::setData(std::string_view path) {
        auto&& [xData, yData, labels, unit, timer_t] =
            benchtools::plotter::PlotDataLoader::LoadFromCSV(path);

        m_xData = std::move(xData);
        m_yData = std::move(yData);
        m_Labels = std::move(labels);
        m_Unit = std::move(unit);
        m_TimerType = std::move(timer_t);
    }

    int PlotWidget::run(int argCount, char** argValues) noexcept {
        if (argCount < 2) {
            std::cerr << "Usage" << argValues[0] << "<csv_file>\n";
            return 1;
        }
        setData(argValues[1]);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, argValues[1], nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        glfwMakeContextCurrent(m_Window);

        // c-style strings of labels for ImPlot
        std::vector<const char*> labelView;
        labelView.reserve(m_Labels.size());

        for (const auto& str : m_Labels)
            labelView.push_back(str.c_str());

        const std::string plotTitle = "Time Durations (" + m_TimerType + ")";
        int displayW{}, displayH{};

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            glfwGetFramebufferSize(m_Window, &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Make the ImGui window cover the whole GLFW client area
            ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            // Begin with no flags (or add NoMove | NoResize if you want to lock it)
            ImGui::Begin(argValues[1], nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

            // Plot fills all available space after the title bar
            ImVec2 plotSize = ImGui::GetContentRegionAvail();

            if (ImPlot::BeginPlot(plotTitle.c_str(), plotSize, ImPlotFlags_None)) {

                ImPlot::SetupAxis(ImAxis_X1, "Timer ID", ImPlotAxisFlags_LockMin);
                std::string yLabel = "Duration (" + m_Unit + ")";
                ImPlot::SetupAxis(ImAxis_Y1, yLabel.c_str(), ImPlotAxisFlags_LockMin);

                ImPlot::SetupAxisTicks(ImAxis_X1, m_xData.data(), (int)m_yData.size(),
                                       labelView.data());

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