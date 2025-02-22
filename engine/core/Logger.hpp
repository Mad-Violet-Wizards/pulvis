#pragma once

#define PULVIS_WARNING_LOG_ENABLED 1
#define PULVIS_INFO_LOG_ENABLED 1

#ifdef RELEASE
    #define PULVIS_DEBUG_ENABLED 0
#else
    #define PULVIS_DEBUG_ENABLED 1
#endif

#include <fmt/color.h>
#include <cstdarg> // dla va_list, va_start, va_end
#include <cstdio>
#include <iostream>

namespace engine::core
{
///////////////////////////////////////////////////////////////////////////////////////////////////
    enum class ELogLevel
    {
        Fatal = 0,
        Error = 1,
        Warning,
        Info,
        Debug
    };

  inline static std::string ToString(ELogLevel _log_level)
    {
        switch(_log_level)
        {
            case ELogLevel::Fatal: return "[FATAL] ";
            case ELogLevel::Error: return "[ERROR] ";
            case ELogLevel::Warning: return "[WARNING] ";
            case ELogLevel::Info: return "[INFO] ";
            case ELogLevel::Debug: return "[DEBUG] ";
        }
        return "";
    }

    inline static std::string ToANSIColoured(ELogLevel _log_level)
    {
        switch(_log_level)
        {
            case ELogLevel::Fatal: return "\033[31m";
            case ELogLevel::Error: return "\033[91m";
            case ELogLevel::Warning: return "\033[93m";
            case ELogLevel::Info: return "\033[94m";
            case ELogLevel::Debug: return "\033[37m";
        }

        return "";
    }
///////////////////////////////////////////////////////////////////////////////////////////////////

    struct SLogInfo
    {
        SLogInfo(ELogLevel _log_level, const std::string& _msg)
					: m_LogLevel(_log_level)
					, m_Message(_msg)
				{

				}

        ELogLevel m_LogLevel;
        std::string m_Message;

        std::string ConsoleDump() const
        {
					return ToANSIColoured(m_LogLevel) + ToString(m_LogLevel) + m_Message + "\033[0m\n";
        }

        std::string FileDump() const
        {
          return "";
        }
    };

    class CLogger : public Singleton<CLogger>, public events::IEventListener
    {
        public:

            CLogger();
            ~CLogger();
             
						CLogger(const CLogger&) = delete;
						CLogger& operator=(const CLogger&) = delete;
						CLogger(CLogger&&) = delete;
						CLogger& operator=(CLogger&&) = delete;

            friend class Singleton<CLogger>;

            void OnEvent(events::IEvent* _event) override;

            template<typename... Args>
            void LOG(ELogLevel _log_level, fmt::format_string<Args...> _msg, Args&&... _args)
            {
              const std::string formatted_msg = fmt::format(_msg, std::forward<Args>(_args)...);
              SLogInfo log_info(_log_level, formatted_msg);
              LogImpl(log_info);
            }

        private:

          void LogImpl(const SLogInfo& _log_info);

      private:
            
				std::queue<SLogInfo> m_LogFileDumpQueue;
    };
}

#ifndef PULVIS_FATAL_LOG
    #define PULVIS_FATAL_LOG(msg, ...) engine::core::CLogger::GetInstance().LOG(engine::core::ELogLevel::Fatal, msg, __VA_ARGS__);
#endif

#ifndef PULVIS_ERROR_LOG
    #define PULVIS_ERROR_LOG(msg, ...) engine::core::CLogger::GetInstance().LOG(engine::core::ELogLevel::Error, msg, __VA_ARGS__);
#endif

#ifdef PULVIS_WARNING_LOG_ENABLED
    #ifndef PULVIS_WARNING_LOG
        #define PULVIS_WARNING_LOG(msg, ...) engine::core::CLogger::GetInstance().LOG(engine::core::ELogLevel::Warning, msg, __VA_ARGS__);
    #endif
#endif

#ifdef PULVIS_INFO_LOG_ENABLED
    #ifndef PULVIS_INFO_LOG
        #define PULVIS_INFO_LOG(msg, ...) engine::core::CLogger::GetInstance().LOG(engine::core::ELogLevel::Info, msg, __VA_ARGS__);
    #endif
#endif

#ifdef PULVIS_DEBUG_ENABLED
    #ifndef PULVIS_DEBUG_LOG
        #define PULVIS_DEBUG_LOG(msg, ...) engine::core::CLogger::GetInstance().LOG(engine::core::ELogLevel::Debug, msg, __VA_ARGS__);
    #endif
#endif