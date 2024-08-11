#include "engine/engine_pch.hpp"
#include "Logger.hpp"

namespace engine
{
namespace core
{
    SLogInfo::SLogInfo(ELogLevel _log_level, const std::string& _msg)
        : m_LogLevel(_log_level)
        , m_Message(_msg)
    {

    }

    std::string SLogInfo::ConsoleDump()
    {
        // TODO: Add DateTime when completed.
        return ToANSIColoured(m_LogLevel) + ToString(m_LogLevel) + m_Message + "\033[0m\n";
    }

    std::string SLogInfo::FileDump()
    {
        // TODO: Add DateTime when completed.
        return ToString(m_LogLevel) + m_Message;
    }
}
}