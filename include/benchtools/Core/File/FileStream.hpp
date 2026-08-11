#pragma once

#include <benchtools/Core/File/File.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace benchtools {

namespace file {

    class FileOStream {
      public:
        FileOStream() noexcept = default;

        explicit FileOStream(std::string_view path, fmode_t mode = fileopen::insert)
            : m_Stream(File(path.data()), mode), m_File(path) {};

        void append(std::string_view content) {
            if (!m_Stream) {
                std::cerr << "Failed to open file\n";
                return;
            }

            m_Stream.write(content.data(), content.size());
            m_Stream.flush();
        }

        [[nodiscard]] operator bool() const { return m_Stream ? 1 : 0; }

      private:
        std::ofstream m_Stream;
        File m_File;
    };

    class FileIStream {
      public:
        explicit FileIStream() noexcept = delete;

        FileIStream(std::string_view path) : m_Stream(File(path.data())), m_File(path) {}

        [[nodiscard]] std::vector<std::string> read() {
            if (!m_Stream) {
                std::cerr << "Failed to open file\n";
                return {};
            }

            std::vector<std::string> res{};

            std::string line{};
            while (std::getline(m_Stream, line))

                res.push_back(line);

            return res;
        };

        [[nodiscard]] operator bool() const { return m_Stream ? 1 : 0; }

      private:
        std::ifstream m_Stream;
        File m_File;
    };
}  // namespace file
}  // namespace benchtools