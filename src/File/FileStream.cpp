#include <benchtools/File/FileStream.hpp>

#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools {

FileOStream::FileOStream(std::string_view path, fmode mode) 
    : m_Stream(std::filesystem::path(path.data()), mode), m_File(path) {}

void FileOStream::append(std::string_view content)  {
    if (!m_Stream) {
        return;
    }
    m_Stream.write(content.data(), content.size());
};

std::vector<std::string> FileIStream::read(uint32_t lines) {
    if (!m_Stream) {
        std::cerr << "Failed to open file\n";
        return {};
    }

    std::vector<std::string> res{};

    std::string line{};
    while (std::getline(m_Stream, line)) {
        res.push_back(line);
    }
    return res;
}

}  // namespace benchtools