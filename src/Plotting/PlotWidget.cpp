#include "benchtools/File/CSVParser.hpp"
#include <benchtools/Plotting/Data/DataLoader.hpp>
#include <benchtools/Plotting/Widgets/PlotWidget.hpp>

#include <execution>
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
        auto&& [inner, labels] = benchtools::plotter::DataLoader::LoadFromCSV(path);
        auto&& [xData, yData] = inner;

        m_xData = std::move(xData);
        m_yData = std::move(yData);
        m_Labels = std::move(labels);
    }

    int PlotWidget::Run(int arg_count, char** arg_values) noexcept {
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
        if (arg_count < 2) {
            std::cerr << "Usage" << arg_values[0] << "<csv_file\n";
            return 1;
        }

        int display_w{}, display_h{};

        std::vector<const char*> label_view;
        label_view.reserve(m_Labels.size());

        std::transform(std::execution::par_unseq, m_Labels.begin(), m_Labels.end(),
                       label_view.begin(), [](std::string& a) { return a.c_str(); });

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            // Framebuffer clearing
            glfwGetFramebufferSize(m_Window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // ImGui window for the plot adjustment
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
            ImGui::Begin(arg_values[1], nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove);

            // Plotting
            if (ImPlot::BeginPlot("Timer Durations")) {
                // Lock axes to first quadrant
                ImPlot::SetupAxis(ImAxis_X1, "Timer ID", ImPlotAxisFlags_LockMin);
                // TODO: somehow extract duration from the csv as well
                ImPlot::SetupAxis(ImAxis_Y1, "Duration", ImPlotAxisFlags_LockMin);

                // Explicit tick positions and labels for discrete X axis
                ImPlot::SetupAxisTicks(ImAxis_X1, m_xData.data(), (int)m_yData.size(),
                                       label_view.data());

                double maxY = *std::max_element(m_yData.begin(), m_yData.end());
                ImPlot::SetupAxesLimits(-1, m_xData.back() + 1, 0, maxY * 1.1);

                // Plot vertical bars (bar width = 0.6)
                ImPlot::PlotBars("Duration", m_xData.data(), m_yData.data(),
                                 (int)m_yData.size(), 0.6);

                ImPlot::EndPlot();
            }

            ImGui::End();

            // Render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_Window);
        }
        return 0;
    }
}  // namespace plotter
}  // namespace benchtools