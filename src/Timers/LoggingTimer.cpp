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

void LoggingTimer::start() noexcept {
    m_Timer->reset();
    m_Timer->start();
}

void LoggingTimer::stop() noexcept {
    m_IsStopped = 1;

    m_Timer->stop();
    BENCHTOOLS_TRACE(static_cast<std::string>(m_Timer->duration(m_Unit)));
}

void LoggingTimer::setUnit(time_unit unit) noexcept {
    m_Unit = unit;
}

}  // namespace benchtools