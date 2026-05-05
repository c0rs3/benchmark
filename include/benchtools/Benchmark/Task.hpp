#pragma once

#include <benchtools/Core/Time.hpp>

#include <functional>

namespace benchtools {

template <typename Signature>
struct Task {
    Task(std::function<Signature> func) noexcept : m_Task(func) {}

    std::function<Signature> m_Task;
};

}  // namespace benchtools