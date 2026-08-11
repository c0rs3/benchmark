#pragma once

#include <benchtools/Core/File/FileStream.hpp>

#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace benchtools::file {

using XMLLine = std::vector<std::string>;
using XMLContent = std::vector<XMLLine>;
using XMLMeta = std::vector<std::pair<std::string, std::string>>;

class XMLStream {

  public:
    explicit XMLStream() noexcept = delete;

    template <class... Args>
    explicit XMLStream(std::string_view path, std::string_view root,
                       Args&&... headers) noexcept
        : m_Stream(path), m_Root(root), m_Headers{std::forward<Args>(headers)...} {
        writeProlog();
        openRoot();
    }

    explicit XMLStream(std::string_view path, std::string_view root,
                       std::vector<std::string> headers) noexcept
        : m_Stream(path), m_Root(root), m_Headers(std::move(headers)) {
        writeProlog();
        openRoot();
    }

    ~XMLStream() noexcept {
        if (m_Stream) {
            closeAll();
        }
    }

    void meta(std::string_view key, std::string_view value) noexcept {
        if (!m_Stream || m_DataOpen) {
            std::cerr << "Cannot write metadata after data rows have started\n";
            return;
        }
        if (!m_MetaOpen) {
            m_Stream.append("  <metadata>\n");
            m_MetaOpen = true;
        }
        m_Stream.append("    <");
        m_Stream.append(key);
        m_Stream.append(">");
        m_Stream.append(escape(value));
        m_Stream.append("</");
        m_Stream.append(key);
        m_Stream.append(">\n");
    }

    void meta(std::string_view rawXML) noexcept {
        if (!m_Stream || m_DataOpen) {
            std::cerr << "Cannot write metadata after data rows have started\n";
            return;
        }
        if (!m_MetaOpen) {
            m_Stream.append("  <metadata>\n");
            m_MetaOpen = true;
        }
        m_Stream.append("    ");
        m_Stream.append(rawXML);
        m_Stream.append("\n");
    }

    template <class... Args>
    void write(Args&&... args) noexcept {
        if (!m_Stream) {
            std::cerr << "Failed to open file\n";
            return;
        }
        XMLLine temp = {(args)...};
        if (temp.size() != m_Headers.size()) {
            std::cerr << "content size does not match header size!\n";
            return;
        }
        ensureData();
        writeRow(temp);
    }

    void write(XMLLine contents) noexcept {
        if (!m_Stream) {
            std::cerr << "Failed to open file\n";
            return;
        }
        if (contents.size() != m_Headers.size()) {
            std::cerr << "content size does not match header size!\n";
            return;
        }
        ensureData();
        writeRow(contents);
    }

    [[nodiscard]] static XMLMeta getMetadata(std::string_view path) {
        file::FileIStream m_Stream{path};
        auto lines = m_Stream.read();
        XMLMeta result;

        bool in_metadata{0};
        for (const auto& line : lines) {
            if (line.find("<metadata>") != std::string::npos) {
                in_metadata = true;
                continue;
            }
            if (line.find("</metadata>") != std::string::npos) {
                in_metadata = false;
                continue;
            }
            if (!in_metadata) continue;

            auto open_tag = line.find('<');
            auto close_tag = line.find('>', open_tag);
            if (open_tag == std::string::npos || close_tag == std::string::npos) continue;

            std::string key = line.substr(open_tag + 1, close_tag - open_tag - 1);
            if (key.empty() || key[0] == '/') continue;

            std::string end_tag = "</" + key + ">";
            auto open_end = line.find(end_tag, close_tag);
            if (open_end == std::string::npos) continue;

            std::string value = line.substr(close_tag + 1, open_end - close_tag - 1);
            result.emplace_back(std::move(key), std::move(value));
        }
        return result;
    }

    [[nodiscard]] static file::XMLLine getHeaders(std::string_view path) {
        file::FileIStream m_Stream{path};
        auto lines = m_Stream.read();

        for (size_t i = 0; i < lines.size(); ++i) {
            if (lines[i].find("<row>") == std::string::npos) continue;

            file::XMLLine headers;
            for (size_t j = i + 1; j < lines.size(); ++j) {
                if (lines[j].find("</row>") != std::string::npos) break;

                auto l_open = lines[j].find('<');
                auto l_close = lines[j].find('>', l_open);
                if (l_open == std::string::npos || l_close == std::string::npos) continue;

                std::string l_tag = lines[j].substr(l_open + 1, l_close - l_open - 1);
                if (!l_tag.empty() && l_tag[0] != '/') {
                    headers.push_back(std::move(l_tag));
                }
            }
            return headers;
        }
        return {};
    }

    [[nodiscard]] static file::XMLContent getRows(std::string_view path) {
        file::FileIStream m_Stream{path};
        auto lines = m_Stream.read();

        file::XMLContent m_Content{};
        file::XMLLine m_Current{};
        bool m_InRow{false};

        for (const auto& line : lines) {
            if (line.find("<row>") != std::string::npos) {
                m_InRow = true;
                m_Current.clear();
                continue;
            }
            if (line.find("</row>") != std::string::npos) {
                m_InRow = false;
                if (!m_Current.empty()) {
                    m_Content.emplace_back(std::move(m_Current));
                }
                continue;
            }
            if (!m_InRow) continue;

            auto l_valStart = line.find('>');
            auto l_valEnd = line.find('<', l_valStart);
            if (l_valStart == std::string::npos || l_valEnd == std::string::npos)
                continue;

            if (l_valEnd > l_valStart + 1) {
                m_Current.emplace_back(
                    line.substr(l_valStart + 1, l_valEnd - l_valStart - 1));
            } else {
                m_Current.emplace_back();
            }
        }
        return m_Content;
    }

  private:
    void writeProlog() {
        m_Stream.append(R"(<?xml version="1.0" encoding="UTF-8"?>)");
        m_Stream.append("\n");
    }

    void openRoot() {
        m_Stream.append("<");
        m_Stream.append(m_Root);
        m_Stream.append(">\n");
    }

    void ensureData() {
        if (!m_DataOpen) {
            if (m_MetaOpen) {
                m_Stream.append("  </metadata>\n");
                m_MetaOpen = false;
            }
            m_Stream.append("  <data>\n");
            m_DataOpen = true;
        }
    }

    void writeRow(const XMLLine& row) {
        m_Stream.append("    <row>\n");
        for (size_t i = 0; i < m_Headers.size(); ++i) {
            m_Stream.append("      <");
            m_Stream.append(m_Headers[i]);
            m_Stream.append(">");
            m_Stream.append(escape(row[i]));
            m_Stream.append("</");
            m_Stream.append(m_Headers[i]);
            m_Stream.append(">\n");
        }
        m_Stream.append("    </row>\n");
    }

    void closeAll() {
        if (m_DataOpen) {
            m_Stream.append("  </data>\n");
        } else if (m_MetaOpen) {
            m_Stream.append("  </metadata>\n");
        }
        m_Stream.append("</");
        m_Stream.append(m_Root);
        m_Stream.append(">\n");
    }

    [[nodiscard]] static std::string escape(std::string_view text) {
        std::string out;
        out.reserve(text.size());
        for (char c : text) {
            switch (c) {
            case '&':
                out += "&amp;";
                break;
            case '<':
                out += "&lt;";
                break;
            case '>':
                out += "&gt;";
                break;
            case '"':
                out += "&quot;";
                break;
            case '\'':
                out += "&apos;";
                break;
            default:
                out += c;
                break;
            }
        }
        return out;
    }

    file::FileOStream m_Stream{};
    std::string m_Root;
    std::vector<std::string> m_Headers{};
    bool m_MetaOpen{false};
    bool m_DataOpen{false};
};

}  // namespace benchtools::file