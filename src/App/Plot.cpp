#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <benchtools/File/CSVParser.hpp>

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

double extract_duration(const std::string& raw) {
    size_t i{};
    while (i < raw.size() && (std::isdigit(raw[i]) || raw[i] == '.' || raw[i] == '-'))
        ++i;
    std::string numberPart = raw.substr(0, i);
    std::string unitPart = raw.substr(i);

    unitPart.erase(unitPart.begin(),
                   std::find_if(unitPart.begin(), unitPart.end(),
                                [](unsigned char ch) { return !std::isspace(ch); }));
    std::transform(unitPart.begin(), unitPart.end(), unitPart.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    double value = std::stod(numberPart);

    if (unitPart == "s" || unitPart.empty())
        return value;
    else if (unitPart == "ms")
        return value / 1'000.0;
    else if (unitPart == "us")
        return value / 1'000'000.0;
    else if (unitPart == "ns")
        return value / 1'000'000'000.0;
    else
        return value;
}

void CreateAxesData(const std::vector<std::array<std::string, 3>>& rows,
                    std::vector<double>& x_data, std::vector<double>& y_data,
                    std::vector<const char*>& labels) {
    x_data.clear();
    y_data.clear();
    labels.clear();
    for (const auto& row : rows) {
        int id = std::stoi(row[0]);
        x_data.push_back(static_cast<double>(id));

        double ms = extract_duration(row[2]);
        y_data.push_back(ms);

        static std::vector<std::string> label_strings;
        label_strings.push_back(row[0]);
        labels.push_back(label_strings.back().c_str());
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage" << argv[0] << "<csv_file\n";
        return 1;
    }

    benchtools::CSVParser<3> parser{argv[1]};
    const auto& rows = parser.getRows();

    std::vector<double> xs, ys;
    std::vector<const char*> labels;
    CreateAxesData(rows, xs, ys, labels);

    if (xs.empty()) {
        std::cerr << "No data found in CSV file.\n";
        return 1;
    }

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImPlot Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Framebuffer clearing
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
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
        ImGui::Begin("Plot Window", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove);

        // Plotting
        if (ImPlot::BeginPlot("Timer Durations")) {
            // Lock axes to first quadrant
            ImPlot::SetupAxis(ImAxis_X1, "Timer ID", ImPlotAxisFlags_LockMin);
            ImPlot::SetupAxis(ImAxis_Y1, "Duration (ms)", ImPlotAxisFlags_LockMin);

            // Explicit tick positions and labels for discrete X axis
            ImPlot::SetupAxisTicks(ImAxis_X1, xs.data(), (int)xs.size(), labels.data());

            double maxY = *std::max_element(ys.begin(), ys.end());
            ImPlot::SetupAxesLimits(-1, xs.back() + 1, 0, maxY * 1.1);

            // Plot vertical bars (bar width = 0.6)
            ImPlot::PlotBars("Duration", xs.data(), ys.data(), (int)xs.size(), 0.6);

            ImPlot::EndPlot();
        }

        ImGui::End();

        // Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}