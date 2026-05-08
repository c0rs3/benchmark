#include "benchtools/Core/File.hpp"
#include <benchtools/File/CSVStream.hpp>

#include <cstddef>
#include <string>
#include <string_view>

namespace benchtools {

template <size_t N>
class CSVParser {
  public:
    CSVParser(std::string_view path) : m_Stream(path) {
        auto&& lines = m_Stream.read();
        m_Headers = parseHeaders<3>(lines[0]);
        for (size_t i{1}; i < lines.size(); i++) {
            m_Content.emplace_back(parseHeaders<3>(lines[i]));
        }
    }
    [[nodiscard]] std::array<std::string, N>& getHeaders() { return m_Headers; }

    [[nodiscard]] std::vector<std::array<std::string, N>>& getRows() { return m_Content; }

  private:
    FileIStream m_Stream;
    std::array<std::string, N> m_Headers{};
    std::vector<std::array<std::string, N>> m_Content{};
};
}  // namespace benchtools