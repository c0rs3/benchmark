#include <benchtools/Loggers/Logger.hpp>
#include <benchtools/Timers/Wrappers/LoggingTimer.hpp>

namespace benchtools {

LoggingTimer::~LoggingTimer() noexcept {
    m_Timer->stop();
    if (!m_IsStopped) {
        BENCHTOOLS_TRACE(static_cast<std::string>(m_Timer->duration(m_Unit)));
    }
    m_Timer->reset();
}

void LoggingTimer::start(bool cond, std::source_location loc) noexcept {
    using namespace std::string_literals;
    m_Timer->reset_if(cond);
    m_Timer->start();
    m_ID = loc;
    std::stringstream ss;
    ss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column() << " "
       << m_ID.function_name() << " Started timer";
    BENCHTOOLS_TRACE(ss.str());
}

void LoggingTimer::stop(std::source_location loc) noexcept {
    m_IsStopped = 1;

    m_Timer->stop();
    std::stringstream ss;
    ss << loc.file_name() << ":" << loc.line() << " "
       << "Timer started at: " << m_ID.line() << ":" << m_ID.column() << " "
       << m_ID.function_name() << ", resulted with:" << " "
       << m_Timer->duration(m_Unit).str();
    BENCHTOOLS_TRACE(ss.str());
}

void LoggingTimer::setUnit(time_unit unit) noexcept {
    m_Unit = unit;
}

}  // namespace benchtools