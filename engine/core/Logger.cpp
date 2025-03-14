#include "engine/engine_pch.hpp"
#include "Logger.hpp"

#include "engine/events/events/FilesystemMountedEvent.hpp"

namespace engine::core
{
	CLogger::CLogger()
	{
	}

	CLogger::~CLogger()
	{
	}

	void CLogger::OnEvent(const events::IEvent* _event)
	{
	}
	void CLogger::LogImpl(const SLogInfo& _log_info)
	{
		std::cout << _log_info.ConsoleDump();
		m_LogFileDumpQueue.push(_log_info);
	}
}