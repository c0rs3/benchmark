#include <benchtools/Core/Benchmark/Result.hpp>

#include <benchtools/Plotter/PlotDataLoader.hpp>
#include <benchtools/Plotter/PlotWidget.hpp>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace benchtools {
namespace plotter {
    PlotWidget::~PlotWidget() noexcept(0) {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }
    }

    PlotWidget& PlotWidget::getInstance() noexcept {
        static PlotWidget widget{};

        return widget;
    }

    void PlotWidget::setData(std::string_view path) noexcept(0) {
        m_Data = PlotData::loadData(path);
    }

    void PlotWidget::run(int argCount, char** argValues) noexcept(0) {
        bool compareMode{false};
        std::vector<std::string_view> paths{};

        if (argCount > 1) {
            std::string_view firstArg{argValues[1]};
            if (firstArg == "--compare") {
                if (argCount < 4)
                    throw std::invalid_argument{
                        "--compare requires at least two file paths"};

                compareMode = true;
                for (int i{2}; i < argCount; ++i)
                    paths.emplace_back(argValues[i]);

            } else
                paths.emplace_back(argValues[1]);
        }

        if (paths.empty()) throw std::invalid_argument{"No benchmark data file provided"};

        setData(paths[0]);

        std::vector<benchmark::Result> compareResults{};
        if (compareMode) {
            compareResults.reserve(paths.size() - 1);
            for (std::size_t i{1}; i < paths.size(); ++i) {
                compareResults.push_back(PlotData::loadData(paths[i]));
            }
        }

        const auto& primary{m_Data};
        const auto& primaryDurations{primary.getDurations()};
        const auto& profile{m_Data.getProfile()};

        auto extractUnit = [](const benchtools::Duration& dur) -> std::string {
            std::ostringstream oss{};
            oss << dur;
            const std::string str{oss.str()};

            std::string unit{};
            for (auto it{str.rbegin()}; it != str.rend(); ++it) {
                if (std::isalpha(static_cast<unsigned char>(*it)))
                    unit.push_back(*it);
                else
                    break;
            }
            if (unit.empty())
                throw std::runtime_error{"Failed to extract unit from duration: " + str};

            std::ranges::reverse(unit);
            return unit;
        };

        const std::string unit{extractUnit(
            primaryDurations.empty() ? benchtools::Duration{} : primaryDurations[0])};

        struct Dataset {
            std::vector<double> xs{};
            std::vector<double> ys{};
            std::vector<double> sortedYs{};
            double meanVal{0.0};
            std::string meanStr{};
        };

        auto buildDataset = [&](const benchmark::Result& result) -> Dataset {
            Dataset ds{};
            const auto& durations{result.getDurations()};
            ds.xs.reserve(durations.size());
            ds.ys.reserve(durations.size());

            for (std::size_t i{0}; i < durations.size(); ++i) {
                ds.xs.push_back(static_cast<double>(i));
                ds.ys.push_back(static_cast<double>(durations[i].count()));
            }

            ds.sortedYs = ds.ys;
            std::ranges::sort(ds.sortedYs);

            ds.meanVal = ds.ys.empty()
                             ? 0.0
                             : std::accumulate(ds.ys.begin(), ds.ys.end(), 0.0) /
                                   static_cast<double>(ds.ys.size());

            std::ostringstream oss{};
            oss << ds.meanVal << ' ' << unit;
            ds.meanStr = oss.str();

            return ds;
        };

        const Dataset primaryDS{buildDataset(primary)};
        std::vector<Dataset> compareDS{};
        compareDS.reserve(compareResults.size());
        for (const auto& result : compareResults) {
            compareDS.push_back(buildDataset(result));
        }

        int maxIterations{static_cast<int>(primaryDS.xs.size())};
        for (const auto& ds : compareDS) {
            maxIterations = std::max(maxIterations, static_cast<int>(ds.xs.size()));
        }

        if (!glfwInit()) {
            throw std::runtime_error{"Failed to initialize GLFW"};
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        constexpr int windowWidth{1920}, windowHeight{1080};
        m_Window = glfwCreateWindow(windowWidth, windowHeight, "Benchmark Plotter",
                                    nullptr, nullptr);
        if (!m_Window) {
            glfwTerminate();
            throw std::runtime_error{"Failed to create GLFW window"};
        }

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();

        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        constexpr int maxTicks{20};
        const int tickStep{std::max(1, maxIterations / maxTicks)};

        std::vector<double> tickVals{};
        std::vector<std::string> tickStrs{};
        tickVals.reserve(static_cast<std::size_t>(maxIterations / tickStep) + 2);
        tickStrs.reserve(static_cast<std::size_t>(maxIterations / tickStep) + 2);

        if (maxIterations > 0) {
            for (int i{0}; i < maxIterations; i += tickStep) {
                tickVals.push_back(static_cast<double>(i));
                tickStrs.push_back(std::to_string(i));
            }
            if (static_cast<int>(tickVals.back()) != maxIterations - 1) {
                tickVals.push_back(static_cast<double>(maxIterations - 1));
                tickStrs.push_back(std::to_string(maxIterations - 1));
            }
        }

        std::vector<const char*> tickCStrs{};
        tickCStrs.reserve(tickStrs.size());
        for (const auto& str : tickStrs) {
            tickCStrs.push_back(str.c_str());
        }

        auto formatDuration = [&](const benchtools::Duration& dur) -> std::string {
            std::ostringstream oss{};
            oss << dur << ' ' << unit;
            return oss.str();
        };

        std::string primaryLabel{paths[0]};
        {
            auto pos{primaryLabel.find_last_of("/\\")};
            if (pos != std::string::npos) {
                primaryLabel = primaryLabel.substr(pos + 1);
            }
        }

        std::vector<std::string> compareLabels{};
        compareLabels.reserve(compareDS.size());
        for (std::size_t i{0}; i < compareDS.size(); ++i) {
            std::string label{"Compare " + std::to_string(i + 1)};
            if (paths.size() > i + 1) {
                std::string_view p{paths[i + 1]};
                auto pos{p.find_last_of("/\\")};
                if (pos != std::string_view::npos) {
                    p = p.substr(pos + 1);
                }
                label = std::string{p};
            }
            compareLabels.push_back(std::move(label));
        }

        bool primaryVisible{true};
        std::vector<char> compareVisible(compareDS.size(), 1);

        constexpr ImVec4 primaryColor{1.0f, 0.5f, 0.0f, 1.0f};
        constexpr std::array<ImVec4, 6> compareColors{{
            {0.0f, 0.7f, 1.0f, 1.0f},
            {0.3f, 1.0f, 0.3f, 1.0f},
            {1.0f, 0.3f, 0.7f, 1.0f},
            {0.8f, 0.8f, 0.2f, 1.0f},
            {0.5f, 0.3f, 1.0f, 1.0f},
            {1.0f, 0.6f, 0.2f, 1.0f},
        }};

        bool logScaleY{false};

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2{0, 0});
            ImGui::SetNextWindowSize(io.DisplaySize);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::Begin("BenchmarkPlotter", nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBringToFrontOnFocus |
                             ImGuiWindowFlags_NoNavFocus);
            ImGui::PopStyleVar();

            if (ImGui::BeginTabBar("BenchmarkTabs", ImGuiTabBarFlags_None)) {

                if (ImGui::BeginTabItem("Overview")) {
                    auto renderProfile = [&](const auto& prof) {
                        if (ImGui::BeginTable("ProfileTable", 2,
                                              ImGuiTableFlags_Borders |
                                                  ImGuiTableFlags_RowBg)) {
                            ImGui::TableSetupColumn(
                                "Property", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                            ImGui::TableSetupColumn("Value",
                                                    ImGuiTableColumnFlags_WidthStretch);
                            ImGui::TableHeadersRow();

                            auto row = [&](const char* key, const char* val) {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(key);
                                ImGui::TableSetColumnIndex(1);
                                ImGui::TextUnformatted(val);
                            };

                            row("Warmup", prof.warmup ? "enabled" : "disabled");
                            row("Warmup Iters",
                                std::to_string(prof.warmupIterations).c_str());
                            row("Iterations", std::to_string(prof.iterations).c_str());
                            row("Policy", benchmark::format(prof.policy).c_str());

                            ImGui::EndTable();
                        }
                    };

                    auto renderStats = [&](const auto& result) {
                        if (ImGui::BeginTable("StatsTable", 2,
                                              ImGuiTableFlags_Borders |
                                                  ImGuiTableFlags_RowBg)) {
                            ImGui::TableSetupColumn(
                                "Metric", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                            ImGui::TableSetupColumn("Value",
                                                    ImGuiTableColumnFlags_WidthStretch);
                            ImGui::TableHeadersRow();

                            auto statRow = [&](const char* name,
                                               const benchtools::Duration& dur) {
                                ImGui::TableNextRow();
                                ImGui::TableSetColumnIndex(0);
                                ImGui::TextUnformatted(name);
                                ImGui::TableSetColumnIndex(1);
                                const auto s{formatDuration(dur)};
                                ImGui::TextUnformatted(s.c_str());
                            };

                            statRow("Min", result.min());
                            statRow("Max", result.max());
                            statRow("Mean", result.mean());
                            statRow("Sum", result.sum());

                            ImGui::EndTable();
                        }
                    };

                    ImGui::TextUnformatted(primaryLabel.c_str());
                    ImGui::Separator();
                    renderProfile(profile);
                    ImGui::Spacing();
                    renderStats(primary);

                    for (std::size_t idx{0}; idx < compareResults.size(); ++idx) {
                        ImGui::Spacing();
                        ImGui::TextUnformatted(compareLabels[idx].c_str());
                        ImGui::Separator();
                        ImGui::PushID(static_cast<int>(idx));
                        renderProfile(compareResults[idx].getProfile());
                        ImGui::Spacing();
                        renderStats(compareResults[idx]);
                        ImGui::PopID();
                    }

                    if (compareMode) {
                        ImGui::Spacing();
                        ImGui::TextUnformatted("Visibility");
                        ImGui::Separator();

                        ImGui::ColorButton("##Primary", primaryColor,
                                           ImGuiColorEditFlags_NoTooltip |
                                               ImGuiColorEditFlags_NoDragDrop,
                                           ImVec2{16, 16});
                        ImGui::SameLine();
                        ImGui::Checkbox("Primary", &primaryVisible);

                        for (std::size_t i{0}; i < compareDS.size(); ++i) {
                            ImGui::PushID(static_cast<int>(i));
                            const auto& col{compareColors[i % compareColors.size()]};
                            ImGui::ColorButton("##CompareColor", col,
                                               ImGuiColorEditFlags_NoTooltip |
                                                   ImGuiColorEditFlags_NoDragDrop,
                                               ImVec2{16, 16});
                            ImGui::SameLine();
                            bool visible{compareVisible[i] != 0};
                            ImGui::Checkbox(compareLabels[i].c_str(), &visible);
                            compareVisible[i] = visible ? 1 : 0;
                            ImGui::PopID();
                        }
                    }

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Runs")) {
                    ImGui::Checkbox("Log Scale Y", &logScaleY);
                    ImVec2 avail{ImGui::GetContentRegionAvail()};
                    const std::string yLabel{std::string{"Duration ("} + unit + ")"};

                    if (ImPlot::BeginPlot("Duration per Run", avail)) {
                        ImPlot::SetupAxis(ImAxis_X1, "Iteration");
                        ImPlot::SetupAxisLimits(ImAxis_X1, -0.5,
                                                static_cast<double>(maxIterations) - 0.5,
                                                ImPlotCond_Always);
                        if (!tickVals.empty()) {
                            ImPlot::SetupAxisTicks(ImAxis_X1, tickVals.data(),
                                                   static_cast<int>(tickVals.size()),
                                                   tickCStrs.data());
                        }
                        ImPlot::SetupAxis(ImAxis_Y1, yLabel.c_str(),
                                          ImPlotAxisFlags_AutoFit);
                        if (logScaleY) {
                            ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
                        }

                        if (primaryVisible && !primaryDS.ys.empty()) {
                            ImPlot::PlotLine("Primary", primaryDS.xs.data(),
                                             primaryDS.ys.data(),
                                             static_cast<int>(primaryDS.xs.size()));

                            const std::array<double, 2> xLimit{
                                -0.5, static_cast<double>(primaryDS.xs.size()) - 0.5};
                            const std::array<double, 2> yLimit{primaryDS.meanVal,
                                                               primaryDS.meanVal};
                            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, primaryColor);
                            ImPlot::PlotLine("Mean", xLimit.data(), yLimit.data(),
                                             static_cast<int>(xLimit.size()));
                            ImPlot::PopStyleColor();
                        }

                        for (std::size_t i{0}; i < compareDS.size(); ++i) {
                            if (!compareVisible[i] || compareDS[i].ys.empty()) {
                                continue;
                            }
                            const auto& col{compareColors[i % compareColors.size()]};
                            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, col);
                            ImPlot::PlotLine(compareLabels[i].c_str(),
                                             compareDS[i].xs.data(),
                                             compareDS[i].ys.data(),
                                             static_cast<int>(compareDS[i].xs.size()));
                            ImPlot::PopStyleColor();

                            const std::array<double, 2> xLimit{
                                -0.5, static_cast<double>(compareDS[i].xs.size()) - 0.5};
                            const std::array<double, 2> yLimit{compareDS[i].meanVal,
                                                               compareDS[i].meanVal};
                            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, col);
                            ImPlot::PlotLine(("Mean (" + compareLabels[i] + ")").c_str(),
                                             xLimit.data(), yLimit.data(),
                                             static_cast<int>(xLimit.size()));
                            ImPlot::PopStyleColor();
                        }

                        ImPlot::EndPlot();
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Distribution")) {
                    ImVec2 avail{ImGui::GetContentRegionAvail()};
                    const std::string xLabel{std::string{"Duration ("} + unit + ")"};

                    if (ImPlot::BeginPlot("Duration Distribution", avail)) {
                        ImPlot::SetupAxes(xLabel.c_str(), "Frequency",
                                          ImPlotAxisFlags_AutoFit,
                                          ImPlotAxisFlags_AutoFit);

                        bool anyVisible{false};
                        if (primaryVisible && !primaryDS.ys.empty()) anyVisible = true;
                        for (std::size_t i{0}; i < compareDS.size(); ++i) {
                            if (compareVisible[i] && !compareDS[i].ys.empty()) {
                                anyVisible = true;
                            }
                        }

                        if (anyVisible) {
                            int refSize{primaryDS.ys.empty()
                                            ? 0
                                            : static_cast<int>(primaryDS.ys.size())};
                            for (const auto& ds : compareDS) {
                                if (!ds.ys.empty()) {
                                    refSize =
                                        std::max(refSize, static_cast<int>(ds.ys.size()));
                                }
                            }
                            const int bins{std::clamp(
                                static_cast<int>(std::sqrt(static_cast<double>(refSize))),
                                10, 100)};

                            if (primaryVisible && !primaryDS.ys.empty()) {
                                ImPlot::PlotHistogram(
                                    "Primary", primaryDS.ys.data(),
                                    static_cast<int>(primaryDS.ys.size()), bins);
                            }

                            for (std::size_t i{0}; i < compareDS.size(); ++i) {
                                if (!compareVisible[i] || compareDS[i].ys.empty()) {
                                    continue;
                                }
                                const auto& col{compareColors[i % compareColors.size()]};
                                ImPlot::PushStyleColor(
                                    ImPlotCol_PlotBorder,
                                    ImVec4{col.x, col.y, col.z, 0.35f});
                                ImPlot::PlotHistogram(
                                    compareLabels[i].c_str(), compareDS[i].ys.data(),
                                    static_cast<int>(compareDS[i].ys.size()), bins);
                                ImPlot::PopStyleColor();
                            }

                            auto limits{ImPlot::GetPlotLimits()};
                            float annotationYOffset{0.95f};

                            if (primaryVisible && !primaryDS.ys.empty()) {
                                ImPlot::Annotation(primaryDS.meanVal,
                                                   limits.Y.Max * annotationYOffset,
                                                   ImVec4{primaryColor.x, primaryColor.y,
                                                          primaryColor.z, 1.0f},
                                                   ImVec2{5, -5}, true, "Mean: %s",
                                                   primaryDS.meanStr.c_str());
                                annotationYOffset -= 0.1f;
                            }

                            for (std::size_t i{0}; i < compareDS.size(); ++i) {
                                if (!compareVisible[i]) continue;
                                const auto& col{compareColors[i % compareColors.size()]};
                                ImPlot::Annotation(compareDS[i].meanVal,
                                                   limits.Y.Max * annotationYOffset,
                                                   ImVec4{col.x, col.y, col.z, 1.0f},
                                                   ImVec2{5, -5}, true, "Mean: %s",
                                                   compareDS[i].meanStr.c_str());
                                annotationYOffset -= 0.1f;
                            }
                        }

                        ImPlot::EndPlot();
                    }
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Sorted")) {
                    ImVec2 avail{ImGui::GetContentRegionAvail()};
                    const std::string yLabel{std::string{"Duration ("} + unit + ")"};

                    if (ImPlot::BeginPlot("Sorted Durations", avail)) {
                        ImPlot::SetupAxis(ImAxis_X1, "Rank");
                        ImPlot::SetupAxisLimits(ImAxis_X1, -0.5,
                                                static_cast<double>(maxIterations) - 0.5,
                                                ImPlotCond_Always);
                        if (!tickVals.empty()) {
                            ImPlot::SetupAxisTicks(ImAxis_X1, tickVals.data(),
                                                   static_cast<int>(tickVals.size()),
                                                   tickCStrs.data());
                        }
                        ImPlot::SetupAxis(ImAxis_Y1, yLabel.c_str(),
                                          ImPlotAxisFlags_AutoFit);

                        if (primaryVisible && !primaryDS.sortedYs.empty()) {
                            std::vector<double> sortedXs(primaryDS.sortedYs.size());
                            std::iota(sortedXs.begin(), sortedXs.end(), 0.0);
                            ImPlot::PlotLine("Primary", sortedXs.data(),
                                             primaryDS.sortedYs.data(),
                                             static_cast<int>(sortedXs.size()));
                        }

                        for (std::size_t i{0}; i < compareDS.size(); ++i) {
                            if (!compareVisible[i] || compareDS[i].sortedYs.empty()) {
                                continue;
                            }
                            const auto& col{compareColors[i % compareColors.size()]};
                            std::vector<double> sortedXs(compareDS[i].sortedYs.size());
                            std::iota(sortedXs.begin(), sortedXs.end(), 0.0);
                            ImPlot::PushStyleColor(ImPlotCol_PlotBorder, col);
                            ImPlot::PlotLine(compareLabels[i].c_str(), sortedXs.data(),
                                             compareDS[i].sortedYs.data(),
                                             static_cast<int>(sortedXs.size()));
                            ImPlot::PopStyleColor();
                        }

                        ImPlot::EndPlot();
                    }
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();

            ImGui::Render();
            int display_w{};
            int display_h{};
            glfwGetFramebufferSize(m_Window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_Window);
        }

        ImPlot::DestroyContext();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
        m_Window = nullptr;
    }
}  // namespace plotter
}  // namespace benchtools