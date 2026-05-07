#pragma once

#include <functional>

namespace benchtools {

template <typename Signature_t>
struct Task {
    explicit Task(std::function<Signature_t> func) noexcept : m_Task(func) {}

    std::function<Signature_t> m_Task;
};

}  // namespace benchtools