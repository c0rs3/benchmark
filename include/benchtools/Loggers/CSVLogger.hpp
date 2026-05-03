#pragma once

#include <benchtools/Core/CSVStream.hpp>

namespace benchtools {
/**
 * @brief
 *
 * @tparam N
 */
template <size_t N> using CSVLogger = CSVStream<N>;

};  // namespace benchtools