#include <benchtools/Core/LogType.hpp>
#include <benchtools/Timers/Wrappers/FileTimer.hpp>

namespace benchtools {
namespace timers {
    void FileTimer::start(std::source_location loc) noexcept {
        m_ID = loc;

        m_Timer->reset();
        m_Timer->start();

        std::stringstream ss;
        ss << m_ID.file_name() << ":" << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << " Started timer";

        m_Logger.Log(ss.str(), LogType::TIMER);
    }

    void FileTimer::stop(bool cond, std::source_location loc) noexcept {
        m_Timer->stop();
        m_Timer->reset_if(cond);

        std::stringstream ss;
        ss << loc.file_name() << ":" << loc.line() << " "
           << "Timer started at: " << m_ID.line() << ":" << m_ID.column() << " "
           << m_ID.function_name() << ", resulted with:" << " "
           << m_Timer->duration(m_Unit).str();

        m_Logger.Log(ss.str(), LogType::TIMER);
    }
};  // namespace timers

}  // namespace benchtools