#pragma once

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace benchtools {

#define BENCHTOOLS_TRACE(...) \
    benchtools::Logger::getInstance().getLogger()->trace(__VA_ARGS__)
#define BENCHTOOLS_INFO(...) \
    benchtools::Logger::getInstance().getLogger()->info(__VA_ARGS__)
#define BENCHTOOLS_WARN(...) \
    benchtools::Logger::getInstance().getLogger()->warn(__VA_ARGS__)
#define BENCHTOOLS_ERR(...) \
    benchtools::Logger::getInstance().getLogger()->error(__VA_ARGS__)
#define BENCHTOOLS_CRITICAL(...) \
    benchtools::Logger::getInstance().getLogger()->critical(__VA_ARGS__)

/**
 * @brief A singleton logger object
 *
 */
class Logger {
  public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    std::shared_ptr<spdlog::logger>& getLogger() { return m_Logger; }

  private:
    Logger() {
        m_Logger = spdlog::stdout_color_mt("GLOBAL");
        m_Logger->set_level(spdlog::level::trace);
    }

    std::shared_ptr<spdlog::logger> m_Logger;
};

}  // namespace benchtools