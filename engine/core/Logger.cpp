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

	void CLogger::OnEvent(events::IEvent* _event)
	{
	}
	void CLogger::LogImpl(const SLogInfo& _log_info)
	{
		_log_info.ConsoleDump();
		m_LogFileDumpQueue.push(_log_info);
	}
}