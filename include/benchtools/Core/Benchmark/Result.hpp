#pragma once

#include <benchtools/Core/Core.hpp>
#include <benchtools/Core/Time.hpp>

#include <benchtools/Core/Benchmark/Policy.hpp>
#include <benchtools/Core/Benchmark/Profile.hpp>

#include <iosfwd>
#include <string>
#include <vector>

namespace benchtools::benchmark {

/**
 * @brief Object for storing benchmark results
 */
class Result {
  public:
    Result();
    explicit Result(std::vector<Duration>&& durations, const Profile& profile);

    [[nodiscard]] Duration mean() const noexcept;
    [[nodiscard]] Duration min() const noexcept;
    [[nodiscard]] Duration max() const noexcept;
    [[nodiscard]] Duration sum() const noexcept;
    [[nodiscard]] const std::vector<Duration>& getDurations() const noexcept;

    void setDurations(std::vector<Duration>&& durations);

    [[nodiscard]] Profile& getProfile() noexcept;

    void deduceMinMaxSum();

    friend std::ostream& operator<<(std::ostream& os, const Result& benchResult);

    [[nodiscard]] explicit operator std::string() const;

  private:
    std::vector<Duration> m_Durations{};
    Profile m_Profile;
    Duration m_minDuration{time::MAX_DUR};
    Duration m_maxDuration{time::MIN_DUR};
    Duration m_sumDuration{time::DEFAULT_DUR};
};

}  // namespace benchtools::benchmark