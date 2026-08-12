#include <benchtools/Core/Benchmark/Result.hpp>

#include <algorithm>
#include <sstream>
#include <utility>

namespace benchtools::benchmark {

Result::Result() = default;

Result::Result(std::vector<Duration>&& durations, const Profile& profile)
    : m_Durations(durations), m_Profile(profile) {
    deduceMinMaxSum();
}

Duration Result::mean() const noexcept {
    return m_sumDuration / m_Profile.iterations;
}

Duration Result::min() const noexcept {
    return m_minDuration;
}

Duration Result::max() const noexcept {
    return m_maxDuration;
}

Duration Result::sum() const noexcept {
    return m_sumDuration;
}

const std::vector<Duration>& Result::getDurations() const noexcept {
    return m_Durations;
}

void Result::setDurations(std::vector<Duration>&& durations) {
    m_Durations = std::move(durations);
}

Profile& Result::getProfile() noexcept {
    return m_Profile;
}

void Result::deduceMinMaxSum() {
    std::for_each(std::begin(m_Durations), std::end(m_Durations), [this](auto& dur) {
        m_sumDuration += dur;
        if (m_maxDuration < dur) m_maxDuration = dur;

        if (m_minDuration > dur) m_minDuration = dur;
    });
}

std::ostream& operator<<(std::ostream& os, const Result& benchResult) {
    const auto& profile = benchResult.m_Profile;
    const auto& durations = benchResult.m_Durations;

    constexpr int W{58};  // inner content width

    // horizontal line/separation
    auto hr = [&](const char* left, const char* right) {
        os << left;
        for (int i = 0; i < W; ++i)
            os << '-';
        os << right << '\n';
    };

    // Build content from any streamable parts, then pad exactly to W
    auto line = [&](auto&&... parts) {
        std::ostringstream content;
        (content << ... << std::forward<decltype(parts)>(parts));
        std::string s = content.str();
        int pad = W - static_cast<int>(s.length());
        if (pad < 0) pad = 0;  // safety: don't crash on overflow
        os << '|' << s << std::string(pad, ' ') << '|' << '\n';
    };

    hr("┌", "┐");
    line("  BENCHMARK RESULTS");
    hr("├", "┤");

    line("  PROFILE");
    line("    Warmup:      ", (profile.warmup ? "enabled" : "disabled"), " (",
         profile.warmupIterations, " iterations)");
    line("    Iterations:  ", profile.iterations);
    line("    Policy:      ", format(profile.policy));
    hr("├", "┤");

    line("  STATISTICS");
    line("    Min:    ", benchResult.min());
    line("    Max:    ", benchResult.max());
    line("    Mean:   ", benchResult.mean());
    line("    Sum:    ", benchResult.sum());
    hr("├", "┤");

    line("  INDIVIDUAL DURATIONS (", durations.size(), " samples)");

    constexpr size_t maxRows{12};
    for (size_t i{}; i < std::min(durations.size(), maxRows); ++i)
        line("    [", i, "]  ", durations[i]);

    if (durations.size() > maxRows)
        line("    ... and ", durations.size() - maxRows, " more ...");

    hr("└", "┘");
    return os;
}

Result::operator std::string() const {
    std::ostringstream oss;
    oss << *this;
    return oss.str();
}

}  // namespace benchtools::benchmark